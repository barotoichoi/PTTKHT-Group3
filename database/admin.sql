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
-- Teacher
('USR_TCH001', 'teacher1', 'teacher123', 'Teacher', 'Active', N'Trần Minh', 'tranminh@edusync.edu.vn', '0901000001', N'Nam', '1980-01-01'),
('USR_TCH002', 'teacher2', 'teacher123', 'Teacher', 'Active', N'Lê Hoa', 'lehoa@edusync.edu.vn', '0901000002', N'Nữ', '1985-05-12'),
-- student
('USR_SV00001', 'sv00001', '123456', 'Student', 'Active', N'Đặng Minh Phúc', 'phucdm0001@student.edu.vn', '0376647378', N'Nam', '2005-01-01'),
('USR_SV00002', 'sv00002', '123456', 'Student', 'Active', N'Huỳnh Văn Cường', 'cuonghv0002@student.edu.vn', '0378830059', N'Nam', '2005-02-02'),
('USR_SV00003', 'sv00003', '123456', 'Student', 'Active', N'Hoàng Diễm Ngọc', 'ngochd0003@student.edu.vn', '0811233066', N'Nữ', '2005-03-03'),
('USR_SV00004', 'sv00004', '123456', 'Student', 'Active', N'Đỗ Văn Nghĩa', 'nghiadv0004@student.edu.vn', '0907058715', N'Nam', '2005-04-04'),
('USR_SV00005', 'sv00005', '123456', 'Student', 'Active', N'Nguyễn Thị Hà', 'hant0005@student.edu.vn', '0322110728', N'Nữ', '2005-05-05'),
('USR_SV00006', 'sv00006', '123456', 'Student', 'Active', N'Huỳnh Thu Hoa', 'hoaht0006@student.edu.vn', '0945672207', N'Nữ', '2005-06-06'),
('USR_SV00007', 'sv00007', '123456', 'Student', 'Active', N'Đỗ Ngọc Hạnh', 'hanhdn0007@student.edu.vn', '0993133911', N'Nữ', '2005-07-07'),
('USR_SV00008', 'sv00008', '123456', 'Student', 'Active', N'Võ Đức Hoàng', 'hoangvd0008@student.edu.vn', '0857863528', N'Nam', '2005-08-08'),
('USR_SV00009', 'sv00009', '123456', 'Student', 'Active', N'Đỗ Hữu An', 'andh0009@student.edu.vn', '0814578657', N'Nam', '2005-09-09'),
('USR_SV00010', 'sv00010', '123456', 'Student', 'Active', N'Võ Ngọc Hà', 'havn0010@student.edu.vn', '0794012707', N'Nữ', '2005-10-10'),
('USR_SV00011', 'sv00011', '123456', 'Student', 'Active', N'Huỳnh Minh Đông', 'donghm0011@student.edu.vn', '0348983995', N'Nam', '2005-11-11'),
('USR_SV00012', 'sv00012', '123456', 'Student', 'Active', N'Vũ Minh Nhân', 'nhanvm0012@student.edu.vn', '0859122555', N'Nam', '2005-12-12'),
('USR_SV00013', 'sv00013', '123456', 'Student', 'Active', N'Hoàng Quang Long', 'longhq0013@student.edu.vn', '0969431212', N'Nam', '2005-01-13'),
('USR_SV00014', 'sv00014', '123456', 'Student', 'Active', N'Huỳnh Quang Nghĩa', 'nghiahq0014@student.edu.vn', '0966520836', N'Nam', '2005-02-14'),
('USR_SV00015', 'sv00015', '123456', 'Student', 'Active', N'Lê Diễm Huyền', 'huyenld0015@student.edu.vn', '0582905786', N'Nữ', '2005-03-15'),
('USR_SV00016', 'sv00016', '123456', 'Student', 'Active', N'Bùi Minh An', 'anbm0016@student.edu.vn', '0923328176', N'Nam', '2005-04-16'),
('USR_SV00017', 'sv00017', '123456', 'Student', 'Active', N'Võ Diễm Linh', 'linhvd0017@student.edu.vn', '0967040440', N'Nữ', '2005-05-17'),
('USR_SV00018', 'sv00018', '123456', 'Student', 'Active', N'Đỗ Thị Linh', 'linhdt0018@student.edu.vn', '0813261522', N'Nữ', '2005-06-18'),
('USR_SV00019', 'sv00019', '123456', 'Student', 'Active', N'Huỳnh Thu Linh', 'linhht0019@student.edu.vn', '0947042870', N'Nữ', '2005-07-19'),
('USR_SV00020', 'sv00020', '123456', 'Student', 'Active', N'Bùi Hữu Khang', 'khangbh0020@student.edu.vn', '0593525967', N'Nam', '2005-08-20'),
('USR_SV00021', 'sv00021', '123456', 'Student', 'Active', N'Phan Diễm Huệ', 'huepd0021@student.edu.vn', '0887428942', N'Nữ', '2005-09-21'),
('USR_SV00022', 'sv00022', '123456', 'Student', 'Active', N'Hoàng Hữu Long', 'longhh0022@student.edu.vn', '0960620495', N'Nam', '2005-10-22'),
('USR_SV00023', 'sv00023', '123456', 'Student', 'Active', N'Phạm Thị Hà', 'hapt0023@student.edu.vn', '0913964622', N'Nữ', '2005-11-23'),
('USR_SV00024', 'sv00024', '123456', 'Student', 'Active', N'Huỳnh Thị Giang', 'gianght0024@student.edu.vn', '0361612072', N'Nữ', '2005-12-24'),
('USR_SV00025', 'sv00025', '123456', 'Student', 'Active', N'Huỳnh Văn Phúc', 'phuchv0025@student.edu.vn', '0341424856', N'Nam', '2005-01-25'),
('USR_SV00026', 'sv00026', '123456', 'Student', 'Active', N'Trần Ngọc Huệ', 'huetn0026@student.edu.vn', '0833922263', N'Nữ', '2005-02-26'),
('USR_SV00027', 'sv00027', '123456', 'Student', 'Active', N'Phạm Hữu Nam', 'namph0027@student.edu.vn', '0938098006', N'Nam', '2005-03-27'),
('USR_SV00028', 'sv00028', '123456', 'Student', 'Active', N'Phan Diễm Loan', 'loanpd0028@student.edu.vn', '0821692349', N'Nữ', '2005-04-28'),
('USR_SV00029', 'sv00029', '123456', 'Student', 'Active', N'Đặng Kim Hiền', 'hiendk0029@student.edu.vn', '0770362448', N'Nữ', '2005-05-29'),
('USR_SV00030', 'sv00030', '123456', 'Student', 'Active', N'Nguyễn Hữu Dũng', 'dungnh0030@student.edu.vn', '0978791890', N'Nam', '2005-06-30'),
('USR_SV00031', 'sv00031', '123456', 'Student', 'Active', N'Trần Quang Đông', 'dongtq0031@student.edu.vn', '0849584498', N'Nam', '2005-07-31'),
('USR_SV00032', 'sv00032', '123456', 'Student', 'Active', N'Hoàng Văn Hào', 'haohv0032@student.edu.vn', '0968319535', N'Nam', '2005-08-15'),
('USR_SV00033', 'sv00033', '123456', 'Student', 'Active', N'Võ Thu Hương', 'huongvt0033@student.edu.vn', '0345611178', N'Nữ', '2005-09-18'),
('USR_SV00034', 'sv00034', '123456', 'Student', 'Active', N'Nguyễn Quang Khang', 'khangnq0034@student.edu.vn', '0900778866', N'Nam', '2005-10-21'),
('USR_SV00035', 'sv00035', '123456', 'Student', 'Active', N'Vũ Hữu Cường', 'cuongvh0035@student.edu.vn', '0343006034', N'Nam', '2005-11-25');

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

INSERT INTO Students (StudentID, UserID, Major, GPA, TuitionOwed) VALUES
('SV00001', 'USR_SV00001', N'Civil Engineering', 3.2, 0),
('SV00002', 'USR_SV00002', N'Graphic Design', 2.9, 4500000),
('SV00003', 'USR_SV00003', N'Accounting', 3.5, 0),
('SV00004', 'USR_SV00004', N'Finance - Banking', 3.1, 0),
('SV00005', 'USR_SV00005', N'Civil Engineering', 2.8, 5000000),
('SV00006', 'USR_SV00006', N'Accounting', 3.6, 0),
('SV00007', 'USR_SV00007', N'Civil Engineering', 3.0, 0),
('SV00008', 'USR_SV00008', N'Information Systems', 3.4, 0),
('SV00009', 'USR_SV00009', N'Software Engineering', 3.7, 0),
('SV00010', 'USR_SV00010', N'Software Engineering', 2.5, 9000000),
('SV00011', 'USR_SV00011', N'Artificial Intelligence', 3.8, 0),
('SV00012', 'USR_SV00012', N'Information Systems', 3.2, 0),
('SV00013', 'USR_SV00013', N'Artificial Intelligence', 3.3, 4500000),
('SV00014', 'USR_SV00014', N'Marketing', 2.7, 0),
('SV00015', 'USR_SV00015', N'Accounting', 3.5, 0),
('SV00016', 'USR_SV00016', N'Marketing', 3.1, 0),
('SV00017', 'USR_SV00017', N'Civil Engineering', 2.9, 0),
('SV00018', 'USR_SV00018', N'Accounting', 3.0, 4500000),
('SV00019', 'USR_SV00019', N'Artificial Intelligence', 3.6, 0),
('SV00020', 'USR_SV00020', N'Software Engineering', 3.4, 0),
('SV00021', 'USR_SV00021', N'Mechanical Engineering', 2.8, 0),
('SV00022', 'USR_SV00022', N'Accounting', 3.3, 0),
('SV00023', 'USR_SV00023', N'Graphic Design', 3.2, 0),
('SV00024', 'USR_SV00024', N'Business Administration', 3.5, 4500000),
('SV00025', 'USR_SV00025', N'Cybersecurity', 3.7, 0),
('SV00026', 'USR_SV00026', N'Information Systems', 3.1, 0),
('SV00027', 'USR_SV00027', N'Graphic Design', 2.6, 9000000),
('SV00028', 'USR_SV00028', N'English Language', 3.4, 0),
('SV00029', 'USR_SV00029', N'Civil Engineering', 3.0, 0),
('SV00030', 'USR_SV00030', N'Business Administration', 3.2, 0),
('SV00031', 'USR_SV00031', N'Information Systems', 3.0, 0),
('SV00032', 'USR_SV00032', N'Business Administration', 3.3, 4500000),
('SV00033', 'USR_SV00033', N'Software Engineering', 3.5, 0),
('SV00034', 'USR_SV00034', N'Information Systems', 2.9, 0),
('SV00035', 'USR_SV00035', N'Computer Science', 3.4, 0);

INSERT INTO Teachers (TeacherID, UserID, DepartmentID, Title) VALUES
('TCH001', 'USR_TCH001', 1, 'Senior Lecturer'),
('TCH002', 'USR_TCH002', 2, 'Lecturer');

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