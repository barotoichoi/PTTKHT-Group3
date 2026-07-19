const express = require("express");
const sql = require("mssql/msnodesqlv8");
const cors = require("cors");

const app = express();

app.use(cors());
app.use(express.json());

const config = {
  connectionString:
    "Driver={ODBC Driver 17 for SQL Server};Server=MSI;Database=StudentManagement;Trusted_Connection=Yes;",
};

async function start() {
  try {
    await sql.connect(config);

    console.log("✅ Connected to SQL Server");
  } catch (err) {
    console.log(err);
  }
}

start();

// lấy all users
app.get("/users", async (req, res) => {
  try {
    const result = await sql.query(`
      SELECT UserID, FullName, Email, Role, Status
      FROM Users
      ORDER BY UserID
    `);

    res.json(result.recordset);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// lấy teacher
app.get("/teachers", async (req, res) => {
  try {
    const result = await sql.query("SELECT * FROM Teachers");

    res.json(result.recordset);
  } catch (err) {
    console.log(err);

    res.status(500).send(err.message);
  }
});

// teacher infomation
app.get("/teachers/:id", async (req, res) => {
  try {
    const teacherID = req.params.id;

    const result = await sql.query`
            SELECT *
            FROM Teachers
            WHERE TeacherID = ${teacherID}
        `;

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

//get all teacher
app.get("/api/teachers/count", async (req, res) => {
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

//get all student
app.get("/api/students/count", async (req, res) => {
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

// Get total courses
app.get("/api/courses/count", async (req, res) => {
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

// Get total tuition paid amount
app.get("/api/tuition/total", async (req, res) => {
  try {
    const result = await sql.query(`
      SELECT ISNULL(SUM(PaidAmount), 0) AS TotalPaidAmount
      FROM Tuition
    `);

    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});

// Get 4 random users
app.get("/api/users/random", async (req, res) => {
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

// Login
// API xử lý đăng nhập (Login)
app.post("/login", async (req, res) => {
  try {
    const { email, password } = req.body;

    // Kiểm tra xem người dùng có gửi đủ email và password không
    if (!email || !password) {
      return res.status(400).json({
        message: "Vui lòng cung cấp đầy đủ Email và Password",
      });
    }

    // Truy vấn kiểm tra thông tin trong bảng Users
    const result = await sql.query`
        SELECT UserID, Role, Status
        FROM Users
        WHERE Email = ${email} AND Password = ${password}
    `;

    // Nếu không tìm thấy user hoặc sai mật khẩu
    if (result.recordset.length === 0) {
      return res.status(401).json({
        message: "Email hoặc mật khẩu không chính xác",
      });
    }

    const user = result.recordset[0];

    // (Tùy chọn) Kiểm tra trạng thái tài khoản
    if (user.Status !== "Active") {
      return res.status(403).json({
        message: "Tài khoản của bạn đang bị khóa hoặc không hoạt động",
      });
    }

    // Xác định trang chuyển hướng dựa trên Role
    let redirectUrl = "";
    if (user.Role === "Student") {
      redirectUrl = "student.html";
    } else if (user.Role === "Teacher") {
      redirectUrl = "teacher.html";
    } else if (user.Role === "Admin") {
      redirectUrl = "admin.html";
    } else {
      return res.status(403).json({
        message: "Quyền truy cập (Role) không hợp lệ",
      });
    }

    // Trả về JSON thành công kèm theo đường dẫn để phía Frontend chuyển trang
    res.json({
      message: "Đăng nhập thành công",
      role: user.Role,
      userId: user.UserID,
      redirectUrl: redirectUrl,
    });
  } catch (err) {
    console.log(err);
    res.status(500).send(err.message);
  }
});
// End login
app.listen(3000, () => {
  console.log("Server running at http://localhost:3000");
});
// Lấy thông tin Dashboard của Sinh viên đang đăng nhập
app.get("/api/student/dashboard/:userId", async (req, res) => {
  try {
    const userId = req.params.userId;
    
    // Nối bảng Users và Students dựa trên UserID
    const result = await sql.query`
        SELECT 
            U.UserID, U.FullName, U.Email, U.Phone, 
            S.StudentID, S.Major, S.GPA
        FROM Users U
        JOIN Students S ON U.UserID = S.UserID
        WHERE U.UserID = ${userId}
    `;

    if (result.recordset.length === 0) {
      return res.status(404).json({ message: "Không tìm thấy thông tin sinh viên" });
    }

    // Trả về dữ liệu sinh viên
    res.json(result.recordset[0]);
  } catch (err) {
    console.error(err);
    res.status(500).send(err.message);
  }
});