const express = require("express");
const router = express.Router();
const sql = require("../config/db");

// =========================
// LOGIN
// =========================
router.post("/login", async (req, res) => {
  try {
    const { email, password } = req.body;

    if (!email || !password) {
      return res.status(400).json({
        message: "Vui lòng cung cấp đầy đủ Email và Password",
      });
    }

    const result = await sql.query`
      SELECT UserID, Role, Status
      FROM Users
      WHERE Email = ${email}
        AND Password = ${password}
    `;

    if (result.recordset.length === 0) {
      return res.status(401).json({
        message: "Email hoặc mật khẩu không chính xác",
      });
    }

    const user = result.recordset[0];

    if (user.Status !== "Active") {
      return res.status(403).json({
        message: "Tài khoản đã bị khóa",
      });
    }

    let redirectUrl = "";

    switch (user.Role) {
      case "Admin":
        redirectUrl = "admin.html";
        break;

      case "Teacher":
        redirectUrl = "teacher.html";
        break;

      case "Student":
        redirectUrl = "student.html";
        break;

      default:
        return res.status(403).json({
          message: "Role không hợp lệ",
        });
    }

    res.json({
      message: "Đăng nhập thành công",
      userId: user.UserID,
      role: user.Role,
      redirectUrl,
    });
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

module.exports = router;
