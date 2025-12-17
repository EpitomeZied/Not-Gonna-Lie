# ASKME

ASKME is a simple command-line application that simulates an anonymous Q&A platform. Users can register, log in, ask questions to other users (optionally anonymously), answer incoming questions, and browse conversations stored in local text files.

## Features
- Register and authenticate users with a username/password pair.
- Send questions to other registered users with optional anonymity.
- Answer questions addressed to you and review answers to questions you asked.
- List all registered usernames and browse incoming/outgoing questions.
- Persist data to plain-text files (`users.txt`, `questions.txt`, `answers.txt`) for an easy, file-based workflow.

## Requirements
- A C++ compiler that supports the standard configured in `CMakeLists.txt` (currently set to C++26).
- CMake 4.0 or later.

## Building
1. Create a build directory:
   ```bash
   mkdir -p build && cd build
   ```
2. Configure the project with CMake:
   ```bash
   cmake ..
   ```
3. Build the executable:
   ```bash
   cmake --build .
   ```

The resulting binary will be available as `ASKME` inside the `build` directory.

## Running
From the `build` directory (after building), run:
```bash
./ASKME
```

On first run, the application will create the required data files (`users.txt`, `questions.txt`, `answers.txt`, and the accompanying ID trackers) in the working directory if they do not already exist. Keep these files in the same directory as the executable to retain application state between runs.

## Usage guide
1. Choose **Register** to create a new account, providing your name, username, and password.
2. Use **Login** to access your account.
3. Inside the session menu:
   - **Questions to me**: View questions you have received and their answers.
   - **Questions from me**: View questions you have sent and any responses.
   - **Answer**: Respond to a question addressed to you.
   - **Ask**: Send a new question to another registered user; you can choose to ask anonymously.
   - **Users**: List all usernames currently registered.
4. Choose **Logout** to return to the main menu or **Exit** to close the program.

## Repository layout
- `main.cpp`: Core application logic for user management, Q&A workflows, and file persistence.
- `CMakeLists.txt`: Build configuration targeting a modern C++ standard.
- `main/`: Directory reserved for build or supporting assets (currently empty in the source tree).
