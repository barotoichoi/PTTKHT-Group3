const express = require("express");
const router = express.Router();

const { sql, poolPromise } = require("../config/db");

// ===============================
// GET ALL TEACHERS
// ===============================
router.get("/", async (req, res) => {
  try {
    const pool = await poolPromise;

    const result = await pool.request().query(`
        SELECT
            t.TeacherID,
            t.UserID,
            u.FullName AS Name,
            d.DepartmentName AS Department,
            u.Email,
            u.Phone,
            u.Username,
            u.Status,
            t.Title
        FROM Teachers t
        JOIN Users u
            ON t.UserID = u.UserID
        LEFT JOIN Departments d
            ON t.DepartmentID = d.DepartmentID
        ORDER BY t.TeacherID
      `);

    res.json(result.recordset);
  } catch (err) {
    console.log(err);
    res.status(500).send(err.message);
  }
});

// ===============================
// GET ONE TEACHER
// ===============================
router.get("/:id", async (req, res) => {
  try {
    const pool = await poolPromise;

    const result = await pool
      .request()
      .input("TeacherID", sql.NVarChar(20), req.params.id).query(`
        SELECT
            t.TeacherID,
            t.UserID,
            u.FullName AS Name,
            d.DepartmentName AS Department,
            u.Email,
            u.Phone,
            u.Username,
            u.Status,
            t.Title

        FROM Teachers t

        JOIN Users u
            ON t.UserID = u.UserID

        LEFT JOIN Departments d
            ON t.DepartmentID = d.DepartmentID

        WHERE t.TeacherID = @TeacherID
      `);

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

// ===============================
// UPDATE TEACHER
// ===============================
router.put("/:id", async (req, res) => {
  try {
    const pool = await poolPromise;

    const t = req.body;

    await pool
      .request()

      .input("FullName", sql.NVarChar(100), t.Name)
      .input("Email", sql.NVarChar(100), t.Email)
      .input("Phone", sql.NVarChar(20), t.Phone)
      .input("Status", sql.NVarChar(20), t.Status)
      .input("UserID", sql.NVarChar(20), t.UserID).query(`
        UPDATE Users
        SET
            FullName = @FullName,
            Email = @Email,
            Phone = @Phone,
            Status = @Status

        WHERE UserID = @UserID
      `);

    res.json({
      success: true,
      message: "Teacher updated successfully",
    });
  } catch (err) {
    console.log(err);
    res.status(500).send(err.message);
  }
});

// ===============================
// DELETE TEACHER
// ===============================
router.delete("/:id", async (req, res) => {
  try {
    const pool = await poolPromise;

    const teacherID = req.params.id;

    const user = await pool
      .request()

      .input("TeacherID", sql.NVarChar(20), teacherID).query(`
        SELECT UserID
        FROM Teachers
        WHERE TeacherID = @TeacherID
      `);

    if (user.recordset.length === 0) {
      return res.status(404).json({
        message: "Teacher not found",
      });
    }

    const userID = user.recordset[0].UserID;

    await pool
      .request()

      .input("TeacherID", sql.NVarChar(20), teacherID).query(`
        DELETE FROM Teachers
        WHERE TeacherID = @TeacherID
      `);

    await pool
      .request()

      .input("UserID", sql.NVarChar(20), userID).query(`
        DELETE FROM Users
        WHERE UserID = @UserID
      `);

    res.json({
      success: true,
      message: "Teacher deleted successfully",
    });
  } catch (err) {
    console.log(err);
    res.status(500).send(err.message);
  }
});

// ===============================
// ADD TEACHER
// ===============================
router.post("/", async (req, res) => {
  const {
    TeacherID,
    Username,
    Password,
    FullName,
    Email,
    Phone,
    Gender,
    DOB,
    DepartmentID,
  } = req.body;

  const pool = await poolPromise;

  const transaction = new sql.Transaction(pool);

  try {
    await transaction.begin();

    // INSERT USERS

    await transaction
      .request()

      .input("UserID", sql.NVarChar(20), TeacherID)
      .input("Username", sql.NVarChar(50), Username)
      .input("Password", sql.NVarChar(100), Password)
      .input("Role", sql.NVarChar(20), "Teacher")
      .input("FullName", sql.NVarChar(100), FullName)
      .input("Email", sql.NVarChar(100), Email)
      .input("Phone", sql.NVarChar(20), Phone)
      .input("Gender", sql.NVarChar(10), Gender)
      .input("DOB", sql.Date, DOB).query(`
        INSERT INTO Users
        (
          UserID,
          Username,
          Password,
          Role,
          FullName,
          Email,
          Phone,
          Gender,
          DOB
        )

        VALUES
        (
          @UserID,
          @Username,
          @Password,
          @Role,
          @FullName,
          @Email,
          @Phone,
          @Gender,
          @DOB
        )
      `);

    // INSERT TEACHERS

    await transaction
      .request()

      .input("TeacherID", sql.NVarChar(20), TeacherID)
      .input("UserID", sql.NVarChar(20), TeacherID)
      .input("DepartmentID", sql.Int, DepartmentID).query(`
        INSERT INTO Teachers
        (
          TeacherID,
          UserID,
          DepartmentID
        )

        VALUES
        (
          @TeacherID,
          @UserID,
          @DepartmentID
        )
      `);

    await transaction.commit();

    res.json({
      success: true,

      message: "Add teacher successfully",
    });
  } catch (err) {
    await transaction.rollback();

    console.log(err);

    res.status(500).json({
      success: false,

      message: err.message,
    });
  }
});

module.exports = router;
