const express = require("express");
const sql = require("mssql/msnodesqlv8");
const cors = require("cors");

const app = express();

app.use(cors());
app.use(express.json());

const config = {
  connectionString:
    "Driver={ODBC Driver 17 for SQL Server};Server=MSI;Database=StudentManagement;Trusted_Connection=Yes;",
};

async function start() {
  try {
    await sql.connect(config);

    console.log("✅ Connected to SQL Server");
  } catch (err) {
    console.log(err);
  }
}

start();

app.get("/teachers", async (req, res) => {
  try {
    const result = await sql.query("SELECT * FROM Teachers");

    res.json(result.recordset);
  } catch (err) {
    console.log(err);

    res.status(500).send(err.message);
  }
});

// teacher infomation
app.get("/teachers/:id", async (req, res) => {
  try {
    const teacherID = req.params.id;

    const result = await sql.query`
            SELECT *
            FROM Teachers
            WHERE TeacherID = ${teacherID}
        `;

    if (result.recordset.length === 0) {
      return res.status(404).json({
        message: "Teacher not found",
      });
    }

    res.json(result.recordset[0]);
  } catch (err) {
    console.log(err);

    res.status(500).send(err.message);
  }
});

//get all teacher
app.get("/api/teachers/count", async (req, res) => {
  try {
    const result = await sql.query(`
        SELECT COUNT(*) AS TotalTeachers
        FROM Teachers
    `);

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

//get all student
app.get("/api/students/count", async (req, res) => {
  try {
    const result = await sql.query(`
        SELECT COUNT(*) AS TotalStudents
        FROM Students
    `);

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

//get all users
// Get all users
app.get("/users", async (req, res) => {
  try {
    const result = await sql.query(`
      SELECT
          UserID,
          FullName,
          Email,
          Role,
          Status
      FROM Users
      ORDER BY UserID
    `);

    res.json(result.recordset);
  } catch (err) {
    console.log(err);
    res.status(500).send(err.message);
  }
});

//get 1 users
// Get user by ID
app.get("/users/:id", async (req, res) => {
  try {
    const id = req.params.id;

    const result = await sql.query`
      SELECT *
      FROM Users
      WHERE UserID = ${id}
    `;

    if (result.recordset.length === 0) {
      return res.status(404).json({
        message: "User not found",
      });
    }

    res.json(result.recordset[0]);
  } catch (err) {
    console.log(err);
    res.status(500).send(err.message);
  }
});

app.listen(3000, () => {
  console.log("Server running at http://localhost:3000");
});
