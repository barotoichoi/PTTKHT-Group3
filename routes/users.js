const express = require("express");
const router = express.Router();
const sql = require("../config/db");

// =========================
// GET ALL USERS
// =========================
router.get("/users", async (req, res) => {
  try {
    const result = await sql.query(`
      SELECT UserID,
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
    const result = await sql.query`
      SELECT *
      FROM Users
      WHERE UserID=${req.params.id}
    `;

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
    const u = req.body;

    await sql.query`
        UPDATE Users
        SET
            FullName=${u.FullName},
            Email=${u.Email},
            Phone=${u.Phone},
            Role=${u.Role},
            Status=${u.Status}
        WHERE UserID=${req.params.id}
    `;

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
    await sql.query`
      DELETE FROM Users
      WHERE UserID=${req.params.id}
    `;

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
    const result = await sql.query(`
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
