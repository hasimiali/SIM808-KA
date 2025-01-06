# SIM808-KA

# Firebase Function for GPS Data POST via SIM808

This repository contains the Firebase function that posts GPS data from an Arduino board with a SIM808 module to Firebase. The function listens to HTTP requests, processes incoming data, and stores it in Firebase Realtime Database.

## Requirements

Before you begin, ensure you have the following:

- **Firebase Project**: Set up a Firebase project at [Firebase Console](https://console.firebase.google.com/).
- **Firebase CLI**: Install the Firebase CLI on your local machine.
  ```bash
  npm install -g firebase-tools
