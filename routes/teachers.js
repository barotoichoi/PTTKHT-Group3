const express = require("express");
const router = express.Router();
const sql = require("../config/db");

// =========================
// GET ALL TEACHERS
// =========================
router.get("/teachers", async (req, res) => {
  try {
    const result = await sql.query(`
      SELECT *
      FROM Teachers
      ORDER BY TeacherID
    `);

    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// =========================
// GET TEACHER BY ID
// =========================
router.get("/teachers/:id", async (req, res) => {
  try {
    const result = await sql.query`
      SELECT *
      FROM Teachers
      WHERE TeacherID=${req.params.id}
    `;

    if (result.recordset.length === 0) {
      return res.status(404).json({
        message: "Teacher not found",
      });
    }

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

module.exports = router;
