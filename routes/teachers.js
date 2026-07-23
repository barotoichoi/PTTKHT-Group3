const express = require("express");
const { sql, poolPromise } = require("../config/db");
const router = express.Router();
//
// GET ALL TEACHERS
//
router.get("/", async (req, res) => {
  try {
    const result = await sql.query(`
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

//
// GET ONE TEACHER
//
router.get("/:id", async (req, res) => {
  try {
    const id = req.params.id;

    const result = await sql.query`
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
      WHERE t.TeacherID = ${id}
    `;

    res.json(result.recordset[0]);
  } catch (err) {
    console.log(err);
    res.status(500).send(err.message);
  }
});

//
// UPDATE TEACHER
//
router.put("/:id", async (req, res) => {
  try {
    const t = req.body;

    await sql.query`
      UPDATE Users
      SET
        FullName = ${t.Name},
        Email = ${t.Email},
        Phone = ${t.Phone},
        Status = ${t.Status}
      WHERE UserID = ${t.UserID}
    `;

    res.sendStatus(200);
  } catch (err) {
    console.log(err);
    res.status(500).send(err.message);
  }
});

//
// DELETE TEACHER
//
router.delete("/:id", async (req, res) => {
  try {
    const id = req.params.id;

    const user = await sql.query`
      SELECT UserID
      FROM Teachers
      WHERE TeacherID = ${id}
    `;

    if (user.recordset.length === 0) return res.sendStatus(404);

    const userID = user.recordset[0].UserID;

    await sql.query`
      DELETE FROM Teachers
      WHERE TeacherID = ${id}
    `;

    await sql.query`
      DELETE FROM Users
      WHERE UserID = ${userID}
    `;

    res.sendStatus(200);
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

    // Insert Users
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

    // Insert Teachers

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
  } catch (error) {
    await transaction.rollback();

    console.log(error);

    res.status(500).json({
      success: false,
      message: error.message,
    });
  }
});

module.exports = router;
