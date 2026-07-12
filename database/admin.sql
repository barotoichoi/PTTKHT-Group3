IF DB_ID('StudentManagement') IS NULL
    CREATE DATABASE StudentManagement;
GO

USE StudentManagement;
GO

-- 1. BẢNG TÀI KHOẢN & THÔNG TIN CÁ NHÂN (Đã gộp Users và Profiles)
IF OBJECT_ID('dbo.Users', 'U') IS NULL
BEGIN
    CREATE TABLE Users (
        UserID NVARCHAR(20) PRIMARY KEY, -- VD: US001, US002...
        Username NVARCHAR(50) UNIQUE NOT NULL,
        Password NVARCHAR(50) NOT NULL,
        Role NVARCHAR(20) CHECK (Role IN ('Student', 'Teacher', 'Admin')),
        Status NVARCHAR(30) DEFAULT 'Active',
        CreatedAt DATETIME DEFAULT GETDATE(),
        
        -- Thông tin cá nhân từ bảng Profiles cũ chuyển sang
        FullName NVARCHAR(100) NOT NULL,
        Email NVARCHAR(100) UNIQUE,
        Phone NVARCHAR(20),
        Gender NVARCHAR(20),
        DOB DATE
    );
END;
GO

-- 2. BẢNG PHÒNG BAN/KHOA
IF OBJECT_ID('dbo.Departments', 'U') IS NULL
BEGIN
    CREATE TABLE Departments (
        DepartmentID INT IDENTITY(1,1) PRIMARY KEY,
        DepartmentName NVARCHAR(100) NOT NULL UNIQUE
    );
END;
GO

-- 3. BẢNG SINH VIÊN (UserID là khóa phụ)
IF OBJECT_ID('dbo.Students', 'U') IS NULL
BEGIN
    CREATE TABLE Students (
        StudentID NVARCHAR(20) PRIMARY KEY, -- Mã sinh viên độc lập (VD: SV202601)
        UserID NVARCHAR(20) UNIQUE NOT NULL, -- Khóa phụ liên kết sang bảng Users
        Major NVARCHAR(100),
        GPA FLOAT CONSTRAINT DF_Students_GPA DEFAULT 0,
        TuitionOwed FLOAT CONSTRAINT DF_Students_TuitionOwed DEFAULT 0,
        FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE
    );
    END;
GO

-- 4. BẢNG GIẢNG VIÊN (UserID là khóa phụ)
IF OBJECT_ID('dbo.Teachers', 'U') IS NULL
BEGIN
    CREATE TABLE Teachers (
        TeacherID NVARCHAR(20) PRIMARY KEY, -- Mã giảng viên độc lập (VD: GV001)
        UserID NVARCHAR(20) UNIQUE NOT NULL,  -- Khóa phụ liên kết sang bảng Users
        DepartmentID INT,
        Title NVARCHAR(50), -- VD: 'Professor', 'Lecturer'
        FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
        FOREIGN KEY (DepartmentID) REFERENCES Departments(DepartmentID) ON DELETE SET NULL
    );
    END;
GO

-- 5. BẢNG MÔN HỌC (Courses)
IF OBJECT_ID('dbo.Courses', 'U') IS NULL
BEGIN
    CREATE TABLE Courses (
        CourseID NVARCHAR(20) PRIMARY KEY,
        CourseName NVARCHAR(150) NOT NULL,
        DepartmentID INT,
        Credits INT DEFAULT 3,
        FOREIGN KEY (DepartmentID) REFERENCES Departments(DepartmentID) ON DELETE SET NULL
    );
    END;
GO

-- 6. BẢNG LỚP HỌC (Classes/Sections)
IF OBJECT_ID('dbo.Classes', 'U') IS NULL
BEGIN
    CREATE TABLE Classes (
        ClassID NVARCHAR(20) PRIMARY KEY,
        CourseID NVARCHAR(20) NOT NULL,
        TeacherID NVARCHAR(20),
        Semester NVARCHAR(30) NOT NULL, -- VD: '2026A'
        MaxCapacity INT DEFAULT 40,
        FOREIGN KEY (CourseID) REFERENCES Courses(CourseID) ON DELETE CASCADE,
        FOREIGN KEY (TeacherID) REFERENCES Teachers(TeacherID) ON DELETE SET NULL
    );
END;
GO

-- 7. BẢNG LỊCH TRÌNH LỚP HỌC
IF OBJECT_ID('dbo.ClassSchedules', 'U') IS NULL
BEGIN
    CREATE TABLE ClassSchedules (
        ScheduleID INT IDENTITY(1,1) PRIMARY KEY,
        ClassID NVARCHAR(20) NOT NULL,
        DayOfWeek NVARCHAR(20) NOT NULL,
        TimeRange NVARCHAR(30) NOT NULL,
        Room NVARCHAR(30),
        FOREIGN KEY (ClassID) REFERENCES Classes(ClassID) ON DELETE CASCADE
    );
    END;
GO

-- 8. BẢNG DANH SÁCH LỚP (Sinh viên đăng ký lớp - Junction Table)
IF OBJECT_ID('dbo.Enrollments', 'U') IS NULL
BEGIN
    CREATE TABLE Enrollments (
        EnrollmentID INT IDENTITY(1,1) PRIMARY KEY,
        ClassID NVARCHAR(20) NOT NULL,
        StudentID NVARCHAR(20) NOT NULL,
        EnrollmentDate DATE DEFAULT GETDATE(),
        Status NVARCHAR(30) DEFAULT 'Enrolled', -- Enrolled, Dropped, Completed
        CONSTRAINT UQ_Class_Student UNIQUE (ClassID, StudentID),
        FOREIGN KEY (ClassID) REFERENCES Classes(ClassID) ON DELETE CASCADE,
        FOREIGN KEY (StudentID) REFERENCES Students(StudentID) ON DELETE CASCADE
    );
END;
GO

-- 9. BẢNG ĐIỂM SỐ (Gắn với Enrollment)
IF OBJECT_ID('dbo.Scores', 'U') IS NULL
BEGIN
    CREATE TABLE Scores (
        ScoreID INT IDENTITY(1,1) PRIMARY KEY,
        EnrollmentID INT NOT NULL,
        AssessmentType NVARCHAR(50) NOT NULL, -- Midterm, Final, Assignment
        Score FLOAT NOT NULL,
        UpdatedAt DATETIME DEFAULT GETDATE(),
        CONSTRAINT UQ_Enrollment_Assessment UNIQUE (EnrollmentID, AssessmentType),
        FOREIGN KEY (EnrollmentID) REFERENCES Enrollments(EnrollmentID) ON DELETE CASCADE
    );
END;
GO

--------------------------------------------------------------------------------
-- 1. NẠP DỮ LIỆU BẢNG: Users (25 Users: 2 Admin, 5 Teacher, 18 Student)
--------------------------------------------------------------------------------
INSERT INTO Users (UserID, Username, Password, Role, Status, FullName, Email, Phone, Gender, DOB) VALUES
-- Admins
('US_AD01', 'admin01', 'admin123', 'Admin', 'Active', N'Nguyễn Văn Quản Lý', 'admin01@school.edu.vn', '0901234561', N'Nam', '1985-05-20'),
('US_AD02', 'admin02', 'admin123', 'Admin', 'Active', N'Lê Thị Hoàng Yến', 'admin02@school.edu.vn', '0901234562', N'Nữ', '1988-11-12'),

INSERT INTO Departments (DepartmentName) VALUES
(N'Computer Science'),
(N'Software Engineering'),
(N'Civil Engineering'),
(N'Graphic Design'),
(N'Accounting'),
(N'Finance - Banking'),
(N'Information Systems'),
(N'Artificial Intelligence'),
(N'Marketing'),
(N'Business Administration');

INSERT INTO Courses (CourseID, CourseName, DepartmentID, Credits) VALUES
('CS101', N'Database Systems', 1, 3),
('CS102', N'Object Oriented Programming', 2, 4),
('CS103', N'Computer Networks', 1, 3),
('CS104', N'Web Application Development', 2, 3),
('CS105', N'Data Structures and Algorithms', 1, 4),
('CS106', N'Artificial Intelligence Concepts', 8, 3),
('BA201', N'Principles of Marketing', 9, 3),
('BA202', N'Introduction to Accounting', 5, 3),
('BA203', N'Corporate Finance', 6, 3),
('BA204', N'Business Management', 10, 3);

INSERT INTO Classes (ClassID, CourseID, TeacherID, Semester, MaxCapacity) VALUES
('CS101-A', 'CS101', 'TCH001', '2026A', 40),
('CS103-A', 'CS103', 'TCH001', '2026A', 40),
('CS105-A', 'CS105', 'TCH001', '2026A', 30),
('CS106-A', 'CS106', 'TCH001', '2026A', 35),
('CS102-A', 'CS102', 'TCH002', '2026A', 35),
('CS104-A', 'CS104', 'TCH002', '2026A', 35),
('BA201-A', 'BA201', 'TCH002', '2026A', 40),
('BA202-A', 'BA202', 'TCH002', '2026A', 45),
('BA203-A', 'BA203', 'TCH001', '2026A', 40),
('BA204-A', 'BA204', 'TCH002', '2026A', 50);

INSERT INTO ClassSchedules (ClassID, DayOfWeek, TimeRange, Room) VALUES
('CS101-A', N'Monday', '07:30 - 09:30', 'A101'),
('CS103-A', N'Friday', '13:00 - 15:00', 'C303'),
('CS105-A', N'Tuesday', '07:30 - 09:30', 'A102'),
('CS106-A', N'Thursday', '10:00 - 12:00', 'A204'),
('CS102-A', N'Wednesday', '09:45 - 11:45', 'B202'),
('CS104-A', N'Thursday', '13:30 - 15:30', 'B205'),
('BA201-A', N'Monday', '10:00 - 12:00', 'C102'),
('BA202-A', N'Tuesday', '13:30 - 15:30', 'C105'),
('BA203-A', N'Wednesday', '15:45 - 17:45', 'A101'),
('BA204-A', N'Friday', '09:45 - 11:45', 'C201');

INSERT INTO Enrollments (ClassID, StudentID, EnrollmentDate, Status) VALUES
('CS101-A', 'SV00001', '2026-01-05', 'Enrolled'),
('CS101-A', 'SV00002', '2026-01-05', 'Enrolled'),
('CS101-A', 'SV00005', '2026-01-05', 'Enrolled'),
('CS101-A', 'SV00007', '2026-01-05', 'Enrolled'),
('CS102-A', 'SV00003', '2026-01-06', 'Enrolled'),
('CS102-A', 'SV00009', '2026-01-06', 'Enrolled'),
('CS102-A', 'SV00010', '2026-01-06', 'Enrolled'),
('CS103-A', 'SV00004', '2026-01-06', 'Enrolled'),
('CS103-A', 'SV00008', '2026-01-06', 'Enrolled'),
('CS104-A', 'SV00005', '2026-01-07', 'Enrolled'),
('CS105-A', 'SV00011', '2026-01-07', 'Enrolled'),
('CS105-A', 'SV00012', '2026-01-07', 'Enrolled'),
('CS106-A', 'SV00013', '2026-01-08', 'Enrolled'),
('CS106-A', 'SV00019', '2026-01-08', 'Enrolled'),
('BA201-A', 'SV00014', '2026-01-08', 'Enrolled'),
('BA201-A', 'SV00016', '2026-01-08', 'Enrolled'),
('BA202-A', 'SV00006', '2026-01-09', 'Enrolled'),
('BA202-A', 'SV00015', '2026-01-09', 'Enrolled'),
('BA203-A', 'SV00018', '2026-01-10', 'Enrolled'),
('BA204-A', 'SV00024', '2026-01-10', 'Enrolled');

INSERT INTO Scores (EnrollmentID, AssessmentType, Score) VALUES
(1, 'Midterm', 7.5),
(1, 'Final', 8.0),
(2, 'Final', 9.0),
(3, 'Midterm', 6.5),
(4, 'Final', 7.0),
(5, 'Assignment', 8.0),
(6, 'Midterm', 8.5),
(7, 'Final', 5.5),
(8, 'Midterm', 7.0),
(10, 'Assignment', 9.0),
(11, 'Midterm', 8.0),
(13, 'Final', 7.5),
(15, 'Midterm', 6.0),
(17, 'Final', 8.5),
(20, 'Assignment', 9.5);
GO