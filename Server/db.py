import firebase_admin
from firebase_admin import credentials, firestore
from firebase_admin import firestore


cred = credentials.Certificate("parking-system-fc488-firebase-adminsdk-q60m6-1b199aeb07.json")
firebase_admin.initialize_app(cred)


MAX_DISTANCE = 9999999



def get_all_parking_lots():
    db = firestore.client()
    parking_lots_ref = db.collection("parking-lots")
    return parking_lots_ref.stream()

def get_parking():
    parking_lots = get_all_parking_lots()
    selected_parking = {'distance_from_gate': MAX_DISTANCE}
    for parking_doc in parking_lots:
        parking = parking_doc.to_dict()
        #print(parking)
        if (not parking["occupied"]) and parking["distance_from_gate"] < selected_parking["distance_from_gate"]:
            selected_parking = parking
    return selected_parking["id"]

def add_parking_request(parking_request):
    db = firestore.client()
    db.collection("parking-requests").add(parking_request)

def remove_parking_request(parking_request):
    db = firestore.client()
    query = db.collection("parking-requests").where("parking_id", "==", parking_request["parking_id"])
    for request in query.stream():
            request.reference.delete()

def is_correct_car_entered_parking(parking_request):
    requests = firestore.client().collection("parking-requests").stream()
    for request in requests:
        if request.to_dict() == parking_request:
            return True
    return False

def update_parking_lots(parking_event):
    db = firestore.client()
    parking_number = parking_event["parking_id"]
    parking_ref = db.collection("parking-lots").document(f"parking{parking_number}")
    data = {
        "current_car_plate" : parking_event["car_number"],
        "occupied" : True,
        "last_time_enter": firestore.SERVER_TIMESTAMP
    }
    parking_ref.update(data)

def free_parking(parking_id):
    db = firestore.client
    parking_ref = db.collection("parking-lots").document(f"parking{parking_id}")
    data = {
        "current_car_plate": "-1",
        "occupied": False
    }
    parking_ref.update(data)

# def temp():
#     db = firestore.client()
#     for i in range(35,36):
#         parking_request = {
#             "current_car_plate" : -1,
#             "distance_from_gate" : 100,
#             "id": i,
#             "last_time_enter": firestore.SERVER_TIMESTAMP,
#             "last_time_exit": firestore.SERVER_TIMESTAMP,
#             "occupied" : False
#         }
#         db.collection("parking-lots").document(f"parking{i}").set(parking_request)