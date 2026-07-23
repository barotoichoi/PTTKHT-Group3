const express = require("express");
const router = express.Router();
const sql = require("mssql");

// Require trực tiếp đối tượng database (pool) từ file db.js
const db = require("../config/db"); 

router.get("/:studentId", async (req, res) => {
  try {
    const { studentId } = req.params;
    
    // Đảm bảo pool đã sẵn sàng (đề phòng db.js export dưới dạng Promise)
    const pool = await db; 
    
    // Sử dụng pool để query trực tiếp
    const result = await pool.request()
      .input("studentId", sql.NVarChar(20), studentId)
      .query(`
        SELECT TuitionID, Semester, Amount, PaidAmount, DueDate, PaymentStatus
        FROM Tuition
        WHERE StudentID = @studentId
        ORDER BY DueDate DESC
      `);

    if (result.recordset.length === 0) {
      return res.status(404).json({ message: "Không tìm thấy dữ liệu học phí" });
    }

    const currentTuition = result.recordset[0];
    const balance = currentTuition.Amount - currentTuition.PaidAmount;
    const progress = (currentTuition.PaidAmount / currentTuition.Amount) * 100;

    res.json({
      currentTuition: {
        ...currentTuition,
        Balance: balance,
        Progress: progress.toFixed(1)
      },
      history: result.recordset 
    });
    
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: "Lỗi Server" });
  }
});

module.exports = router;