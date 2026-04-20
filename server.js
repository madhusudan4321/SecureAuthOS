const express = require("express");
const fs = require("fs");
const path = require("path");
const cors = require("cors");

const app = express();
app.use(cors());
app.use(express.json());
app.use(express.static("frontend"));

const USERS = path.join(__dirname, "data/users.txt");
const LOGS  = path.join(__dirname, "data/logs.txt");

if (!fs.existsSync(path.join(__dirname, "data"))) {
    fs.mkdirSync(path.join(__dirname, "data"));
}

function hashPassword(password) {
    let hash = 0;
    for (let i = 0; i < password.length; i++) {
        hash = (hash * 31 + password.charCodeAt(i)) | 0;
    }
    return hash;
}

function log(msg) {
    const now = new Date().toLocaleString();
    fs.appendFileSync(LOGS, `[${now}] ${msg}\n`);
}

function readUsers() {
    if (!fs.existsSync(USERS)) return [];
    return fs.readFileSync(USERS, "utf8")
        .trim()
        .split("\n")
        .filter(Boolean)
        .map(line => {
            const [username, hash] = line.split(" ");
            return { username, hash: Number(hash) };
        });
}

function validateInput(username, password) {
    if (!username || !password) return "Please fill in all fields.";
    if (username.length < 3)    return "Username must be at least 3 characters.";
    if (password.length < 6)    return "Password must be at least 6 characters.";
    if (!/^[a-zA-Z0-9_]+$/.test(username)) return "Username can only contain letters, numbers, and underscores.";
    return null;
}

app.post("/register", (req, res) => {
    const { username, password } = req.body;
    const error = validateInput(username, password);
    if (error) return res.json({ ok: false, msg: error });

    const users = readUsers();
    if (users.some(u => u.username === username))
        return res.json({ ok: false, msg: "Username is already taken." });

    fs.appendFileSync(USERS, `${username} ${hashPassword(password)}\n`);
    log(`REGISTER: ${username}`);
    res.json({ ok: true, msg: "Account created! You can now sign in." });
});

app.post("/login", (req, res) => {
    const { username, password } = req.body;
    if (!username || !password)
        return res.json({ ok: false, msg: "Please fill in all fields." });

    const users = readUsers();
    const hash  = hashPassword(password);
    const found = users.find(u => u.username === username && u.hash === hash);

    if (!found)
        return res.json({ ok: false, msg: "Incorrect username or password." });

    const otp = String(Math.floor(1000 + Math.random() * 9000));
    log(`LOGIN_PASSWORD_OK: ${username}`);
    res.json({ ok: true, msg: "OTP generated.", otp });
});

app.post("/verify", (req, res) => {
    const { username } = req.body;
    log(`LOGIN_COMPLETE: ${username}`);
    res.json({ ok: true });
});

app.listen(3000, () =>
    console.log("Server running → http://localhost:3000")
);