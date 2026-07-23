const express = require("express");
const cors = require("cors");

const app = express();

app.use(cors());
app.use(express.json());

// Database
require("./config/db");

// Routes
app.use(require("./routes/users"));
app.use(require("./routes/dashboard"));
app.use(require("./routes/auth"));

const studentRoutes = require("./routes/students");
app.use("/api/students", studentRoutes);

const teacherRoutes = require("./routes/teachers");
app.use("/teachers", teacherRoutes);

app.listen(3000, () => {
  console.log("Server running at http://localhost:3000");
});
