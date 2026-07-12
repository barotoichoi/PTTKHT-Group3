/*=========================================================
    TEACHER.SQL: Insert Data for Teachers
    Order: Users -> Teachers -> Classes -> ClassSchedules
=========================================================*/

USE StudentManagement;
GO

/*=========================================================
    1. INSERT INTO USERS (Teachers)
=========================================================*/

INSERT INTO Users (UserID, Username, Password, Role, Status, FullName, Email, Phone, Gender, DOB)
VALUES
    ('T001', 'nguyenthong', 'pass123456', 'Teacher', 'Active', N'Nguyễn Thông', 'nguyenthong@school.edu', '0912345678', N'Nam', '1980-05-15'),
    ('T002', 'phuongtrinh', 'pass123456', 'Teacher', 'Active', N'Phương Trinh', 'phuongtrinh@school.edu', '0912345679', N'Nữ', '1985-03-22'),
    ('T003', 'hungquang', 'pass123456', 'Teacher', 'Active', N'Hùng Quang', 'hungquang@school.edu', '0912345680', N'Nam', '1982-07-10'),
    ('T004', 'linhduc', 'pass123456', 'Teacher', 'Active', N'Linh Đức', 'linhduc@school.edu', '0912345681', N'Nữ', '1988-01-30'),
    ('T005', 'minh_khanh', 'pass123456', 'Teacher', 'Active', N'Minh Khánh', 'minh.khanh@school.edu', '0912345682', N'Nam', '1986-11-05');
GO

/*=========================================================
    2. INSERT INTO DEPARTMENTS (if not already exists)
=========================================================*/

IF NOT EXISTS (SELECT 1 FROM Departments WHERE DepartmentName = N'Khoa Toán')
    INSERT INTO Departments (DepartmentName) VALUES (N'Khoa Toán');

IF NOT EXISTS (SELECT 1 FROM Departments WHERE DepartmentName = N'Khoa Lý')
    INSERT INTO Departments (DepartmentName) VALUES (N'Khoa Lý');

IF NOT EXISTS (SELECT 1 FROM Departments WHERE DepartmentName = N'Khoa Hóa')
    INSERT INTO Departments (DepartmentName) VALUES (N'Khoa Hóa');

IF NOT EXISTS (SELECT 1 FROM Departments WHERE DepartmentName = N'Khoa Văn')
    INSERT INTO Departments (DepartmentName) VALUES (N'Khoa Văn');

GO

/*=========================================================
    3. INSERT INTO TEACHERS
=========================================================*/

INSERT INTO Teachers (TeacherID, UserID, DepartmentID, Title)
VALUES
    ('T001', 'T001', (SELECT DepartmentID FROM Departments WHERE DepartmentName = N'Khoa Toán'), N'Tiến sĩ'),
    ('T002', 'T002', (SELECT DepartmentID FROM Departments WHERE DepartmentName = N'Khoa Lý'), N'Thạc sĩ'),
    ('T003', 'T003', (SELECT DepartmentID FROM Departments WHERE DepartmentName = N'Khoa Hóa'), N'Tiến sĩ'),
    ('T004', 'T004', (SELECT DepartmentID FROM Departments WHERE DepartmentName = N'Khoa Văn'), N'Thạc sĩ'),
    ('T005', 'T005', (SELECT DepartmentID FROM Departments WHERE DepartmentName = N'Khoa Toán'), N'Giảng viên');
GO

/*=========================================================
    4. INSERT INTO CLASSES
=========================================================*/

INSERT INTO Classes (ClassID, CourseID, TeacherID, Semester, MaxCapacity)
VALUES
    ('C001', 'MATH101', 'T001', N'2024-1', 40),
    ('C002', 'MATH102', 'T001', N'2024-1', 40),
    ('C003', 'PHYS101', 'T002', N'2024-1', 35),
    ('C004', 'CHEM101', 'T003', N'2024-1', 35),
    ('C005', 'LIT101', 'T004', N'2024-1', 40),
    ('C006', 'MATH201', 'T005', N'2024-1', 30);
GO

/*=========================================================
    5. INSERT INTO CLASS SCHEDULES
=========================================================*/

INSERT INTO ClassSchedules (ClassID, DayOfWeek, TimeRange, Room)
VALUES
    ('C001', N'Thứ 2', '08:00-09:30', N'Phòng 101'),
    ('C001', N'Thứ 4', '08:00-09:30', N'Phòng 101'),
    ('C002', N'Thứ 3', '10:00-11:30', N'Phòng 102'),
    ('C002', N'Thứ 5', '10:00-11:30', N'Phòng 102'),
    ('C003', N'Thứ 2', '13:00-14:30', N'Phòng 201'),
    ('C003', N'Thứ 5', '13:00-14:30', N'Phòng 201'),
    ('C004', N'Thứ 3', '14:45-16:15', N'Phòng 202'),
    ('C004', N'Thứ 6', '14:45-16:15', N'Phòng 202'),
    ('C005', N'Thứ 2', '16:30-18:00', N'Phòng 301'),
    ('C005', N'Thứ 4', '16:30-18:00', N'Phòng 301'),
    ('C006', N'Thứ 3', '09:00-10:30', N'Phòng 103'),
    ('C006', N'Thứ 6', '09:00-10:30', N'Phòng 103');
GO

/*=========================================================
    DONE: Teacher data inserted successfully
=========================================================*/

PRINT 'Dữ liệu giáo viên đã được insert thành công!';
GO
