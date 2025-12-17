#include <bits/stdc++.h>
using namespace std;

class Entity {
protected:
    int id;

public:
    Entity(int id = 0) : id(id) {}

    int get_id() const {
        return id;
    }

    virtual ~Entity() = default;
};

class User : public Entity {
private:
    string name;
    string username;
    string password;

public:
    User(int id, string name, string username, string password)
        : Entity(id), name(name), username(username), password(password) {}

    User() = default;

    string get_name() const { return name; }
    string get_username() const { return username; }
    string get_password() const { return password; }
};

class Question : public Entity {
private:
    string from_user;
    string to_user;
    bool state;
    string text;

public:
    Question(int id, string from_user, string to_user, bool state, string text)
        : Entity(id), from_user(from_user),
          to_user(to_user), state(state), text(text) {}

    string get_from_user() const { return from_user; }
    string get_to_user() const { return to_user; }
    bool get_state() const { return state; }
    string get_text() const { return text; }
};

class Answer : public Entity {
private:
    int question_id;
    string text;

public:
    Answer(int id, int question_id, string text)
        : Entity(id), question_id(question_id), text(text) {}

    int get_question_id() const { return question_id; }
    string get_text() const { return text; }
};

string user_path = "./users.txt";
string question_path = "./questions.txt";
string answer_path = "./answers.txt";
string user_id_path = "./user_id.txt";
string question_id_path = "./question_id.txt";
string answer_id_path = "./answer_id.txt";

vector<User> users;
vector<Question> questions;
vector<Answer> answers;
User cur_user;

vector<string> split_csv(const string &s, char delim = ',') {
    vector<string> res;
    string temp;
    stringstream ss(s);
    while (getline(ss, temp, delim))
        res.push_back(temp);
    return res;
}

void load_users() {
    ifstream in(user_path);
    string line;
    while (getline(in, line)) {
        auto v = split_csv(line);
        users.emplace_back(stoi(v[0]), v[1], v[2], v[3]);
    }
}

void load_questions() {
    ifstream in(question_path);
    string line;
    while (getline(in, line)) {
        auto v = split_csv(line);
        questions.emplace_back(stoi(v[0]), v[1], v[2], v[3][0]-'0', v[4]);
    }
}

void load_answers() {
    ifstream in(answer_path);
    string line;
    while (getline(in, line)) {
        auto v = split_csv(line);
        answers.emplace_back(stoi(v[0]), stoi(v[1]), v[2]);
    }
}

int generate_id(const string &path) {
    int id = 0;
    ifstream in(path);
    in >> id;
    in.close();
    ofstream out(path);
    out << id + 1;
    return id + 1;
}

void print_question_to_me() {
    bool found = false;
    for (auto &q : questions) {
        if (q.get_to_user() == cur_user.get_username()) {
            found = true;
            cout << "Question ID [" << q.get_id() << "]: ";
            if (q.get_state())
                cout << "Anonymous: ";
            else
                cout << q.get_from_user() << ": ";
            cout << q.get_text() << "\n";

            bool answered = false;
            for (auto &a : answers) {
                if (a.get_question_id() == q.get_id()) {
                    cout << "\tAnswer: " << a.get_text() << "\n";
                    answered = true;
                }
            }
            if (!answered)
                cout << "\tNot answered yet\n";
        }
    }
    if (!found) cout << "No questions for you\n";
}

void print_question_from_me() {
    bool found = false;
    for (auto &q : questions) {
        if (q.get_from_user() == cur_user.get_username()) {
            found = true;
            cout << "Question ID [" << q.get_id() << "] to "
                 << q.get_to_user() << ": "
                 << q.get_text() << "\n";

            bool answered = false;
            for (auto &a : answers) {
                if (a.get_question_id() == q.get_id()) {
                    cout << "\tAnswer: " << a.get_text() << "\n";
                    answered = true;
                }
            }
            if (!answered)
                cout << "\tNo answer yet\n";
        }
    }
    if (!found) cout << "You asked no questions\n";
}

void answer_question() {
    int qid;
    cout << "Enter question id (0 to exit): ";
    cin >> qid;
    if (qid == 0) return;

    for (auto &q : questions) {
        if (q.get_id() == qid && q.get_to_user() == cur_user.get_username()) {
            for (auto &a : answers)
                if (a.get_question_id() == qid) {
                    cout << "Already answered\n";
                    return;
                }
            string text;
            cout << "Your answer: ";
            getline(cin >> ws, text);
            int id = generate_id(answer_id_path);
            answers.emplace_back(id, qid, text);
            ofstream out(answer_path, ios::app);
            out << id << "," << qid << "," << text << "\n";
            cout << "Answered successfully\n";
            return;
        }
    }
    cout << "Invalid question id\n";
}

void ask_question() {
    string to, text;
    bool anon;
    cout << "Ask user: ";
    cin >> to;

    bool exists = false;
    for (auto &u : users)
        if (u.get_username() == to && to != cur_user.get_username())
            exists = true;
    if (!exists) {
        cout << "User not found\n";
        return;
    }

    cout << "Question: ";
    getline(cin >> ws, text);
    cout << "Anonymous? (1/0): ";
    cin >> anon;

    int id = generate_id(question_id_path);
    questions.emplace_back(id, cur_user.get_username(), to, anon, text);
    ofstream out(question_path, ios::app);
    out << id << "," << cur_user.get_username()
        << "," << to << "," << anon << "," << text << "\n";
    cout << "Question sent\n";
}

void list_users() {
    for (auto &u : users)
        cout << u.get_username() << "\n";
}

void show_menu() {
    while (true) {
        cout << "\n1- Questions to me\n2- Questions from me\n3- Answer\n4- Ask\n5- Users\n6- Logout\nChoice: ";
        int c;
        cin >> c;
        if (c == 1) print_question_to_me();
        else if (c == 2) print_question_from_me();
        else if (c == 3) answer_question();
        else if (c == 4) ask_question();
        else if (c == 5) list_users();
        else if (c == 6) break;
    }
}

void login() {
    string u, p;
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    for (auto &user : users)
        if (user.get_username() == u && user.get_password() == p) {
            cur_user = user;
            show_menu();
            return;
        }
    cout << "Invalid login\n";
}

void Register() {
    string name, u, p;
    cout << "Name: ";
    getline(cin >> ws, name);
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    int id = generate_id(user_id_path);
    users.emplace_back(id, name, u, p);
    ofstream out(user_path, ios::app);
    out << id << "," << name << "," << u << "," << p << "\n";
    cout << "Registered successfully\n";
}

int main() {
    load_users();
    load_questions();
    load_answers();

    while (true) {
        cout << "\n1- Login\n2- Register\n0- Exit\nChoice: ";
        int c;
        cin >> c;
        if (c == 1) login();
        else if (c == 2) Register();
        else if (c == 0) break;
    }
}
