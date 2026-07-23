const express = require("express");
const router = express.Router();

const { poolPromise } = require("../config/db");

// 1. API lấy Tổng quan (Current GPA & Total Credits)
router.get("/api/student/:userId/grades/summary", async (req, res) => {
  try {
    const userId = req.params.userId;

    const pool = await poolPromise;

    const result = await pool.request().input("UserID", userId).query(`
        SELECT 
          s.GPA as CurrentGPA,
          ISNULL(SUM(c.Credits), 0) as TotalCredits
        FROM Students s
        LEFT JOIN Enrollments e ON s.StudentID = e.StudentID
        LEFT JOIN Classes cl ON e.ClassID = cl.ClassID
        LEFT JOIN Courses c ON cl.CourseID = c.CourseID
        WHERE s.UserID = @UserID 
          AND e.Status = 'Enrolled'
        GROUP BY s.GPA
      `);

    if (result.recordset.length === 0) {
      return res.json({
        CurrentGPA: 0,
        TotalCredits: 0,
      });
    }

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// 2. API lấy GPA Trend
router.get("/api/student/:userId/grades/trend", async (req, res) => {
  try {
    const userId = req.params.userId;

    const pool = await poolPromise;

    const result = await pool.request().input("UserID", userId).query(`
        SELECT 
          cl.Semester,
          CAST(AVG(sc.Score) AS DECIMAL(10,2)) as SemesterGPA
        FROM Students s
        JOIN Enrollments e ON s.StudentID = e.StudentID
        JOIN Classes cl ON e.ClassID = cl.ClassID
        JOIN Scores sc ON e.EnrollmentID = sc.EnrollmentID
        WHERE s.UserID = @UserID
        GROUP BY cl.Semester
        ORDER BY cl.Semester
      `);

    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// 3. API lấy bảng điểm chi tiết
router.get("/api/student/:userId/grades/details", async (req, res) => {
  try {
    const userId = req.params.userId;

    const pool = await poolPromise;

    const result = await pool.request().input("UserID", userId).query(`
        SELECT 
          c.CourseID,
          c.CourseName,
          c.Credits,
          cl.Semester,
          CAST(AVG(sc.Score) AS DECIMAL(10,2)) as AverageScore
        FROM Students s
        JOIN Enrollments e ON s.StudentID = e.StudentID
        JOIN Classes cl ON e.ClassID = cl.ClassID
        JOIN Courses c ON cl.CourseID = c.CourseID
        JOIN Scores sc ON e.EnrollmentID = sc.EnrollmentID
        WHERE s.UserID = @UserID
        GROUP BY 
          c.CourseID,
          c.CourseName,
          c.Credits,
          cl.Semester
        ORDER BY cl.Semester DESC
      `);

    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

module.exports = router;
