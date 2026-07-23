const express = require("express");
const router = express.Router();

const { poolPromise } = require("../config/db");

// =========================
// GET ALL USERS
// =========================
router.get("/users", async (req, res) => {
  try {
    const pool = await poolPromise;

    const result = await pool.request().query(`
        SELECT 
            UserID,
            FullName,
            Email,
            Phone,
            Role,
            Status
        FROM Users
        ORDER BY UserID
      `);

    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// =========================
// GET USER BY ID
// =========================
router.get("/users/:id", async (req, res) => {
  try {
    const pool = await poolPromise;

    const result = await pool
      .request()

      .input("UserID", req.params.id).query(`
        SELECT *
        FROM Users
        WHERE UserID = @UserID
      `);

    if (result.recordset.length === 0) {
      return res.status(404).json({
        message: "User not found",
      });
    }

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// =========================
// UPDATE USER
// =========================
router.put("/users/:id", async (req, res) => {
  try {
    const pool = await poolPromise;

    const u = req.body;

    await pool
      .request()

      .input("UserID", req.params.id)
      .input("FullName", u.FullName)
      .input("Email", u.Email)
      .input("Phone", u.Phone)
      .input("Role", u.Role)
      .input("Status", u.Status).query(`
        UPDATE Users
        SET
            FullName = @FullName,
            Email = @Email,
            Phone = @Phone,
            Role = @Role,
            Status = @Status

        WHERE UserID = @UserID
      `);

    res.json({
      success: true,

      message: "User updated successfully",
    });
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// =========================
// DELETE USER
// =========================
router.delete("/users/:id", async (req, res) => {
  try {
    const pool = await poolPromise;

    await pool
      .request()

      .input("UserID", req.params.id).query(`
        DELETE FROM Users
        WHERE UserID = @UserID
      `);

    res.json({
      success: true,

      message: "User deleted successfully",
    });
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// =========================
// RANDOM USERS
// =========================
router.get("/api/users/random", async (req, res) => {
  try {
    const pool = await poolPromise;

    const result = await pool.request().query(`
        SELECT TOP 4
            UserID,
            FullName,
            Role,
            Status

        FROM Users

        ORDER BY NEWID()
      `);

    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

module.exports = router;
