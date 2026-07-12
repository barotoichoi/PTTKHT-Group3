/*=========================================================
    INSERT ADMIN ACCOUNT
=========================================================*/

INSERT INTO Users
(
    UserID,
    Username,
    Password,
    Role,
    Status,
    FullName,
    Email,
    Phone,
    Gender,
    DOB
)
VALUES
(
    'AD001',
    'admin',
    '123456',      -- Có thể đổi thành mật khẩu khác
    'Admin',
    'Active',
    N'Quản trị viên',
    'admin@student.edu.vn',
    '0900000000',
    N'Nam',
    '2000-01-01'
);
GO