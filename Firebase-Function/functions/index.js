const { onRequest } = require("firebase-functions/v2/https");
const admin = require("firebase-admin");
const logger = require("firebase-functions/logger");

// Initialize Firebase Admin SDK
admin.initializeApp();

// Define the HTTP function that posts data to Firestore
exports.postDataToFirestore = onRequest(async (request, response) => {
    // Log the incoming request data for debugging
    logger.info("Incoming request data:", request.body);

    try {
        // Ensure only POST requests are allowed
        if (request.method !== 'POST') {
            return response.status(405).send('Method Not Allowed');
        }

        // Get the data from the request body (Assumes JSON format)
        const data = request.body;

        // Make sure data exists and is an object
        if (!data || typeof data !== 'object') {
            return response.status(400).send('Invalid data format');
        }

        // Save data to Firestore
        const docRef = admin.firestore().collection('data').doc();
        await docRef.set(data);

        // Respond with success message
        return response.status(200).send('Data posted successfully');
    } catch (error) {
        // Log and send error response
        logger.error('Error posting data to Firestore:', error);
        return response.status(500).send('Internal Server Error');
    }
});
