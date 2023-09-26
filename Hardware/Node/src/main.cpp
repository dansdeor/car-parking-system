#include "binary_ir_sensor.h"
#include "camera.h"
#include "header.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <iostream>

#ifdef GATE
#include "oled_display.h"
#else
#include "led_strip.h"
#endif

#define MAIN_LOOP_DELAY 1000
#define AFTER_MAIN_LOOP_DELAY 10000
#define BEFORE_TAKE_SHOT_DELAY 500
#define WIFI_CHECK_STATUS_DELAY 500
#define BEFORE_RECEIVING_SERVER_RESPONSE 5000

String construct_image_json(const String& captured_image)
{
    String json_data = "{\"node_id\":\"" + String(NODE_ID) + "\",\"image\":\"" + captured_image + "\"}";
    return json_data;
}

String construct_json()
{
    String json_data = "{\"node_id\":\"" + String(NODE_ID) + "\"}";
    return json_data;
}

String http_post_json(String url_path, String json_data, t_http_codes* http_code_ptr)
{
    HTTPClient http;
    http.begin(SERVER_URL + url_path);
    http.setTimeout(BEFORE_RECEIVING_SERVER_RESPONSE);
    http.addHeader("Content-Type", "application/json");
    t_http_codes http_code = (t_http_codes)http.POST(json_data);
    delay(AFTER_MAIN_LOOP_DELAY);
    if (http_code_ptr) {
        *http_code_ptr = http_code;
    }
    Serial.print("http code: ");
    Serial.println(http_code);
    String response = http.getString();
    http.end();
    return response;
}

void setup()
{
    Serial.begin(115200);
    camera_init();
#ifdef GATE
    oled_display_init();
#else
    led_strip_init();
#endif
    binary_ir_sensor_init();

    WiFi.begin(SSID_NAME, SSID_PASSWORD);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_CHECK_STATUS_DELAY);
    }
    Serial.print("\nIP: ");
    Serial.println(WiFi.localIP());
    http_post_json("awake", construct_json(), NULL);
}

void loop()
{
    delay(MAIN_LOOP_DELAY);
    bool static object_recognized = false;

    if (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_CHECK_STATUS_DELAY);
        return;
    }

    if (!binary_ir_object_detected()) {
#ifdef GATE
        oled_display_clear();
#else
        led_strip_set_color(CRGB::Blue);
#endif
        if (object_recognized) {
            http_post_json("car-left", construct_json(), NULL);
            object_recognized = false;
        }
        return;
    }
    if (object_recognized) {
        return;
    }
    delay(BEFORE_TAKE_SHOT_DELAY);
    Serial.println("Object detected");

    String base64_image;
    if (!camera_get_base64_image(base64_image)) {
        Serial.println("Camera capture failed");
        return;
    }

    String json_data = construct_image_json(base64_image);
    Serial.print("finished contructiong json, size:");
    Serial.println(json_data.length());

    t_http_codes http_code;
    String response = http_post_json("image", json_data, &http_code);
    //delay(AFTER_MAIN_LOOP_DELAY);
    switch (http_code) {
    case HTTP_CODE_OK:
#ifdef GATE
        // Show the parking slot in the oled->the response is the number
        Serial.print("Parking slot: ");
        Serial.println(response);
        oled_display_show(response);
#else
        led_strip_set_color(CRGB::Yellow);
#endif
        object_recognized = true;
        break;
    case HTTP_CODE_BAD_REQUEST:
        Serial.print("Bad request: ");
        Serial.println(response);
        break;
    case HTTP_CODE_NOT_FOUND:
        // Need to take a picture again
        // Do nothing
        break;
    case HTTP_CODE_NOT_ACCEPTABLE:
#ifdef GATE
        // Show the parking slot in the oled->the response
        // is the X maybe
        oled_display_show("X"); // TODO:Talk to Itay or Tom
#else
        led_strip_set_color(CRGB::Red);
#endif
        object_recognized = true;
        break;
    default:
        delay(BEFORE_RECEIVING_SERVER_RESPONSE);
        break;
    }
}
