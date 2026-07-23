const express = require("express");
const router = express.Router();

const { sql, poolPromise } = require("../config/db");

router.get("/api/student/:userId", async (req, res) => {
  try {
    const result = await sql.query`
      SELECT
          U.UserID,
          U.FullName,
          U.Email,
          U.Phone,
          S.StudentID,
          S.Major,
          S.GPA
      FROM Users U
      JOIN Students S
           ON U.UserID = S.UserID
      WHERE U.UserID = ${req.params.userId}
    `;

    if (result.recordset.length === 0)
      return res.status(404).json({
        message: "Không tìm thấy sinh viên",
      });

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

router.get("/api/student/:userId/classes", async (req, res) => {
  try {
    const userId = req.params.userId;
    const result = await sql.query`
      SELECT 
        c.ClassID,
        co.CourseName,
        (SELECT COUNT(*) FROM Enrollments e2 WHERE e2.ClassID = c.ClassID AND e2.Status = 'Enrolled') AS StudentCount,
        (SELECT TOP 1 Room FROM ClassSchedules cs WHERE cs.ClassID = c.ClassID) AS Room,
        
        -- TÍNH TIẾN ĐỘ THẬT: (Số cột điểm hiện có của sinh viên / 3 cột điểm chuẩn) * 100
        ISNULL(
          (SELECT COUNT(*) FROM Scores sc WHERE sc.EnrollmentID = e.EnrollmentID) * 100 / 3
        , 0) AS Progress

      FROM Enrollments e
      JOIN Students s ON e.StudentID = s.StudentID
      JOIN Classes c ON e.ClassID = c.ClassID
      JOIN Courses co ON c.CourseID = co.CourseID
      WHERE s.UserID = ${userId} AND e.Status = 'Enrolled'
    `;
    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

router.get("/api/student/:userId/schedule/today", async (req, res) => {
  try {
    const userId = req.params.userId;
    // Dùng DATENAME(dw, GETDATE()) của SQL Server để tự động lấy thứ của ngày hiện tại
    const result = await sql.query`
      SELECT 
        co.CourseName,
        cs.TimeRange,
        cs.Room
      FROM Enrollments e
      JOIN Students s ON e.StudentID = s.StudentID
      JOIN Classes c ON e.ClassID = c.ClassID
      JOIN Courses co ON c.CourseID = co.CourseID
      JOIN ClassSchedules cs ON c.ClassID = cs.ClassID
      WHERE s.UserID = ${userId} 
        AND e.Status = 'Enrolled'
        AND cs.DayOfWeek = DATENAME(dw, GETDATE())
      ORDER BY cs.TimeRange
    `;
    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

router.get("/api/student/:userId/course-details", async (req, res) => {
  try {
    const userId = req.params.userId;

    const result = await sql.query`
      SELECT 
          c.CourseID,
          c.CourseName,
          tu.FullName AS TeacherName,
          
          -- Lấy phòng học đầu tiên tìm thấy trong lịch
          (SELECT TOP 1 cs.Room FROM ClassSchedules cs WHERE cs.ClassID = cl.ClassID) AS Room,
          
          -- Nối các ngày học lại với nhau (VD: Monday, Wednesday)
          (SELECT STRING_AGG(cs.DayOfWeek, ', ') FROM ClassSchedules cs WHERE cs.ClassID = cl.ClassID) AS ScheduleDays,
          
          -- Lấy giờ bắt đầu (cắt 5 ký tự đầu từ chuỗi VD: '07:30-09:30')
          (SELECT TOP 1 LEFT(cs.TimeRange, 5) FROM ClassSchedules cs WHERE cs.ClassID = cl.ClassID) AS StartTime,
          
          -- Tính Tiến độ tương tự logic đã dùng: (Số cột điểm hiện có / 3 cột điểm chuẩn) * 100
          ISNULL(
            (SELECT COUNT(*) FROM Scores sc WHERE sc.EnrollmentID = e.EnrollmentID) * 100 / 3
          , 0) AS Progress

      FROM Enrollments e
      JOIN Students s ON e.StudentID = s.StudentID
      JOIN Classes cl ON e.ClassID = cl.ClassID
      JOIN Courses c ON cl.CourseID = c.CourseID
      LEFT JOIN Teachers t ON cl.TeacherID = t.TeacherID
      LEFT JOIN Users tu ON t.UserID = tu.UserID
      WHERE s.UserID = ${userId} AND e.Status = 'Enrolled'
    `;

    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

router.get("/", async (req, res) => {
  try {
    const result = await sql.query`

        SELECT
            S.StudentID,
            U.FullName AS Name,
            S.Major,
            S.GPA,
            U.Email,
            U.Phone,
            U.Gender,
            U.DOB,
            U.Status

        FROM Students S

        JOIN Users U
        ON S.UserID = U.UserID

        ORDER BY S.StudentID

        `;

    res.json(result.recordset);
  } catch (err) {
    console.error(err);

    res.status(500).json({
      message: err.message,
    });
  }
});

module.exports = router;
