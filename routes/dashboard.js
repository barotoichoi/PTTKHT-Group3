const express = require("express");
const router = express.Router();
const sql = require("../config/db");

// =========================
// TOTAL TEACHERS
// =========================
router.get("/api/teachers/count", async (req, res) => {
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

// =========================
// TOTAL STUDENTS
// =========================
router.get("/api/students/count", async (req, res) => {
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

// =========================
// TOTAL COURSES
// =========================
router.get("/api/courses/count", async (req, res) => {
  try {
    const result = await sql.query(`
      SELECT COUNT(*) AS TotalCourses
      FROM Courses
    `);

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// =========================
// TOTAL TUITION
// =========================
router.get("/api/tuition/total", async (req, res) => {
  try {
    const result = await sql.query(`
      SELECT ISNULL(SUM(PaidAmount),0) AS TotalPaidAmount
      FROM Tuition
    `);

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

module.exports = router;
