-- 1. BẢNG QUẢN LÝ TÀI KHOẢN TRUNG TÂM (Central Auth)
-- Loại bỏ việc lưu Username/Password ở từng bảng riêng lẻ
CREATE TABLE Users (
    UserID NVARCHAR(20) PRIMARY KEY, -- VD: ST001, TC001, AD001
    Username NVARCHAR(50) UNIQUE NOT NULL,
    Password NVARCHAR(50) NOT NULL,
    Role NVARCHAR(20) CHECK (Role IN ('Student', 'Teacher', 'Admin')),
    Status NVARCHAR(30) DEFAULT 'Active',
    CreatedAt DATETIME DEFAULT GETDATE()
);

-- 2. BẢNG THÔNG TIN CÁ NHÂN (Kế thừa từ Users)
CREATE TABLE Profiles (
    UserID NVARCHAR(20) PRIMARY KEY,
    FullName NVARCHAR(100) NOT NULL,
    Email NVARCHAR(100) UNIQUE,
    Phone NVARCHAR(20),
    Gender NVARCHAR(20),
    DOB DATE,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE
);

-- 3. BẢNG PHÒNG BAN/KHOA
CREATE TABLE Departments (
    DepartmentID INT IDENTITY(1,1) PRIMARY KEY,
    DepartmentName NVARCHAR(100) NOT NULL UNIQUE
);

-- 4. BẢNG SINH VIÊN (Chỉ chứa dữ liệu chuyên biệt của sinh viên)
CREATE TABLE Students (
    StudentID NVARCHAR(20) PRIMARY KEY,
    Major NVARCHAR(100),
    GPA FLOAT CONSTRAINT DF_Students_GPA DEFAULT 0,
    TuitionOwed FLOAT CONSTRAINT DF_Students_TuitionOwed DEFAULT 0,
    FOREIGN KEY (StudentID) REFERENCES Profiles(UserID) ON DELETE CASCADE
);

-- 5. BẢNG GIẢNG VIÊN (Chỉ chứa dữ liệu chuyên biệt của giảng viên)
CREATE TABLE Teachers (
    TeacherID NVARCHAR(20) PRIMARY KEY,
    DepartmentID INT,
    Title NVARCHAR(50), -- VD: 'Professor', 'Lecturer'
    FOREIGN KEY (TeacherID) REFERENCES Profiles(UserID) ON DELETE CASCADE,
    FOREIGN KEY (DepartmentID) REFERENCES Departments(DepartmentID) ON DELETE SET NULL
);

-- 6. BẢNG MÔN HỌC (Courses)
CREATE TABLE Courses (
    CourseID NVARCHAR(20) PRIMARY KEY,
    CourseName NVARCHAR(150) NOT NULL,
    DepartmentID INT,
    Credits INT DEFAULT 3,
    FOREIGN KEY (DepartmentID) REFERENCES Departments(DepartmentID) ON DELETE SET NULL
);

-- 7. BẢNG LỚP HỌC (Classes/Sections)
CREATE TABLE Classes (
    ClassID NVARCHAR(20) PRIMARY KEY,
    CourseID NVARCHAR(20) NOT NULL,
    TeacherID NVARCHAR(20),
    Semester NVARCHAR(30) NOT NULL, -- VD: '2026A'
    MaxCapacity INT DEFAULT 40,
    FOREIGN KEY (CourseID) REFERENCES Courses(CourseID) ON DELETE CASCADE,
    FOREIGN KEY (TeacherID) REFERENCES Teachers(TeacherID) ON DELETE SET NULL
);

-- 8. BẢNG LỊCH TRÌNH LỚP HỌC (Tách từ bảng Teachers)
CREATE TABLE ClassSchedules (
    ScheduleID INT IDENTITY(1,1) PRIMARY KEY,
    ClassID NVARCHAR(20) NOT NULL,
    DayOfWeek NVARCHAR(20) NOT NULL,
    TimeRange NVARCHAR(30) NOT NULL,
    Room NVARCHAR(30),
    FOREIGN KEY (ClassID) REFERENCES Classes(ClassID) ON DELETE CASCADE
);

-- 9. BẢNG DANH SÁCH LỚP (Sinh viên đăng ký lớp - Junction Table)
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

-- 10. BẢNG ĐIỂM SỐ (Gắn với Enrollment thay vì gộp chung lộn xộn)
CREATE TABLE Scores (
    ScoreID INT IDENTITY(1,1) PRIMARY KEY,
    EnrollmentID INT NOT NULL,
    AssessmentType NVARCHAR(50) NOT NULL, -- Midterm, Final, Assignment
    Score FLOAT NOT NULL,
    UpdatedAt DATETIME DEFAULT GETDATE(),
    CONSTRAINT UQ_Enrollment_Assessment UNIQUE (EnrollmentID, AssessmentType),
    FOREIGN KEY (EnrollmentID) REFERENCES Enrollments(EnrollmentID) ON DELETE CASCADE
);