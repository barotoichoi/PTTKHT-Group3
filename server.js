const express = require("express");
const cors = require("cors");

const app = express();

app.use(cors());
app.use(express.json());

// Kết nối Database
require("./config/db");

// Routes
app.use(require("./routes/users"));
app.use(require("./routes/teachers"));
app.use(require("./routes/dashboard"));
app.use(require("./routes/auth"));
app.use(require("./routes/students"));

app.listen(3000, () => {
  console.log("Server running at http://localhost:3000");
});
