const mongoose = require("mongoose");

// Define a schema for sensor data
const sensorDataSchema = new mongoose.Schema({
    sensorId: String,
    
    temperature: Number,
    humidity: Number,
    // ozone: Number,
    // carbonMonoxide: Number,
    waterQuality: Number,
    timestamp: { type: Date, default: Date.now },
});

module.exports = sensorDataSchema;