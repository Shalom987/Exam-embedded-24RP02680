# Smart Water Management System

## Overview
The **Smart Water Management System** is designed to help users remotely monitor and manage their daily water consumption efficiently. The system ensures that users can track their water usage, set consumption limits, and manage their balance via an online platform.

## Project Objectives
- **Monitor Water Usage:** Users can track their real-time water consumption.
- **Balance Management:** Users can top up their balance and avoid excessive water usage.
- **Automated Water Control:** The system automatically closes the valve when the balance reaches zero or exceeds the daily limit.
- **Offline Edge Processing:** Ensures decisions are made locally, even when internet connectivity is lost.

## Key Features
- **Real-time Monitoring:** Users can view their water consumption and remaining balance.
- **Automated Water Flow Control:** The system shuts off water when limits are exceeded.
- **Multi-user Simulation:** Three houses are connected to a single ESP32 microcontroller.
- **LED Indicators:**
  - **Red LED**: Balance is zero (valve closed).
  - **Green LED**: Balance is above zero (valve open).
- **Cloud Integration:** Data is stored and retrieved from Google Cloud.

## Prototype Simulation
The project simulation involves:
- **Using ESP32 in VS Code with Wokwi Simulator.**
- **Three Buttons per House:**
  - **Increase Balance Button:** Simulates a top-up.
  - **Decrease Balance Button:** Simulates water usage.
- **Google Cloud Database:** Stores user balances and water usage records.

## System Deployment
- **Hosted on GitHub Pages:** [GitHub Hosted Link](https://shalom987.github.io/Exam-embedded-24RP02680/)
- **Future Enhancements:** The system will be further improved by transitioning from Wokwi to Proteus, integrating a flow sensor, keypad entry, and LCD for real-time monitoring.

## Future Improvements
- **Hardware:**
  - Integrate a flow sensor to track actual water usage.
  - Implement a keypad for balance entry instead of buttons.
  - Use an LCD display to show real-time balance information.
- **Software:**
  - Improve cloud storage efficiency for better data logging.
  - Implement mobile app integration for better user accessibility.
- **Platform Enhancements:**
  - Consider using a dedicated IoT server for better scalability.
  - Implement predictive analytics for water consumption trends.

## Contributors
This project is developed as part of the **Embedded System Development** module in the **Information Technology** program.

## License
This project is licensed under the **MIT License**.

