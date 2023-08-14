#include "binary_ir_sensor.h"
#include "header.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define MAIN_LOOP_DELAY 500
#define WIFI_CHECK_STATUS_DELAY 1000

String construct_json(const char* captured_image)
{
    String json_data = "{\"node_id\":\"" + String(NODE_ID) + "\",\"image\":\"" + String(captured_image) + "\"}";
    Serial.println(json_data); // TODO: delete this later
    return json_data;
}

String http_post_json(String json_data, t_http_codes* http_code)
{
    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    String response = "";
    if (http_code) {
        *http_code = (t_http_codes)http.POST(json_data);
    }
    response = http.getString();
    Serial.print("http code: ");
    Serial.println(*http_code);
    return response;
}

void setup()
{
    Serial.begin(115200);
    binary_ir_sensor_init();
    WiFi.begin(SSID_NAME, SSID_PASSWORD);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("\nIP: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    bool static object_recognized = false;
    delay(MAIN_LOOP_DELAY);
    if (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_CHECK_STATUS_DELAY);
        return;
    }
    bool object_detected = binary_ir_object_detected();
    if (!object_detected) {
        // turn the leds to blue
        object_recognized = false;
        return;
    }
    if (object_recognized) {
        return;
    }

    Serial.println("Object detected");
    //  take_picture();
    //  check picture
    //  and encode it to base64
    String json_data = construct_json("image buffer in base64");
    t_http_codes http_code;
    String response = http_post_json(json_data, &http_code);
    switch (http_code) {
    case HTTP_CODE_OK:
        // Show the parking slot in the oled->the response is the number
        // and turn the leds to yellow
        Serial.print("Parking slot: ");
        Serial.println(response);
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
        // Show the parking slot in the oled->the response
        // is the X maybe
        // TODO:Talk to Itay or Tom
        // Turns the leds to red
        object_recognized = true;
        break;
    default:
        break;
    }
}
