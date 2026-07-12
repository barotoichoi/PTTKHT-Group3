/*=========================================================
    DATABASE: StudentManagement
=========================================================*/

USE master;
GO

IF DB_ID('StudentManagement') IS NOT NULL
BEGIN
    ALTER DATABASE StudentManagement SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
    DROP DATABASE StudentManagement;
END
GO

CREATE DATABASE StudentManagement;
GO

USE StudentManagement;
GO

/*=========================================================
    1. USERS
=========================================================*/

CREATE TABLE Users
(
    UserID NVARCHAR(20) PRIMARY KEY,

    Username NVARCHAR(50) NOT NULL UNIQUE,
    Password NVARCHAR(100) NOT NULL,

    Role NVARCHAR(20) NOT NULL
        CHECK(Role IN ('Student','Teacher','Admin')),

    Status NVARCHAR(30)
        DEFAULT 'Active',

    CreatedAt DATETIME
        DEFAULT GETDATE(),

    FullName NVARCHAR(100) NOT NULL,

    Email NVARCHAR(100) UNIQUE,

    Phone NVARCHAR(20),

    Gender NVARCHAR(20),

    DOB DATE
);
GO

/*=========================================================
    2. DEPARTMENTS
=========================================================*/

CREATE TABLE Departments
(
    DepartmentID INT IDENTITY(1,1) PRIMARY KEY,

    DepartmentName NVARCHAR(100)
        NOT NULL UNIQUE
);
GO

/*=========================================================
    3. STUDENTS
=========================================================*/

CREATE TABLE Students
(
    StudentID NVARCHAR(20) PRIMARY KEY,

    UserID NVARCHAR(20)
        NOT NULL UNIQUE,

    Major NVARCHAR(100),

    GPA FLOAT
        DEFAULT 0,

    TuitionOwed FLOAT
        DEFAULT 0,

    CONSTRAINT FK_Students_Users
        FOREIGN KEY(UserID)
        REFERENCES Users(UserID)
        ON DELETE CASCADE
);
GO

/*=========================================================
    4. TEACHERS
=========================================================*/

CREATE TABLE Teachers
(
    TeacherID NVARCHAR(20) PRIMARY KEY,

    UserID NVARCHAR(20)
        NOT NULL UNIQUE,

    DepartmentID INT,

    Title NVARCHAR(50),

    CONSTRAINT FK_Teachers_Users
        FOREIGN KEY(UserID)
        REFERENCES Users(UserID)
        ON DELETE CASCADE,

    CONSTRAINT FK_Teachers_Departments
        FOREIGN KEY(DepartmentID)
        REFERENCES Departments(DepartmentID)
        ON DELETE SET NULL
);
GO
/*=========================================================
    5. COURSES
=========================================================*/

CREATE TABLE Courses
(
    CourseID NVARCHAR(20) PRIMARY KEY,

    CourseName NVARCHAR(150) NOT NULL,

    DepartmentID INT,

    Credits INT DEFAULT 3,

    CONSTRAINT FK_Courses_Departments
        FOREIGN KEY(DepartmentID)
        REFERENCES Departments(DepartmentID)
        ON DELETE SET NULL
);
GO

/*=========================================================
    6. CLASSES
=========================================================*/

CREATE TABLE Classes
(
    ClassID NVARCHAR(20) PRIMARY KEY,

    CourseID NVARCHAR(20) NOT NULL,

    TeacherID NVARCHAR(20),

    Semester NVARCHAR(30) NOT NULL,

    MaxCapacity INT DEFAULT 40,

    CONSTRAINT FK_Classes_Courses
        FOREIGN KEY(CourseID)
        REFERENCES Courses(CourseID)
        ON DELETE CASCADE,

    CONSTRAINT FK_Classes_Teachers
        FOREIGN KEY(TeacherID)
        REFERENCES Teachers(TeacherID)
        ON DELETE SET NULL
);
GO

/*=========================================================
    7. CLASS SCHEDULES
=========================================================*/

CREATE TABLE ClassSchedules
(
    ScheduleID INT IDENTITY(1,1) PRIMARY KEY,

    ClassID NVARCHAR(20) NOT NULL,

    DayOfWeek NVARCHAR(20) NOT NULL,

    TimeRange NVARCHAR(30) NOT NULL,

    Room NVARCHAR(30),

    CONSTRAINT FK_ClassSchedules_Classes
        FOREIGN KEY(ClassID)
        REFERENCES Classes(ClassID)
        ON DELETE CASCADE
);
GO
/*=========================================================
    8. ENROLLMENTS
=========================================================*/

CREATE TABLE Enrollments
(
    EnrollmentID INT IDENTITY(1,1) PRIMARY KEY,

    ClassID NVARCHAR(20) NOT NULL,

    StudentID NVARCHAR(20) NOT NULL,

    EnrollmentDate DATE DEFAULT GETDATE(),

    Status NVARCHAR(30) DEFAULT 'Enrolled',

    CONSTRAINT UQ_Class_Student
        UNIQUE(ClassID, StudentID),

    CONSTRAINT FK_Enrollments_Classes
        FOREIGN KEY(ClassID)
        REFERENCES Classes(ClassID)
        ON DELETE CASCADE,

    CONSTRAINT FK_Enrollments_Students
        FOREIGN KEY(StudentID)
        REFERENCES Students(StudentID)
        ON DELETE CASCADE
);
GO

/*=========================================================
    9. SCORES
=========================================================*/

CREATE TABLE Scores
(
    ScoreID INT IDENTITY(1,1) PRIMARY KEY,

    EnrollmentID INT NOT NULL,

    AssessmentType NVARCHAR(50) NOT NULL,

    Score FLOAT NOT NULL,

    UpdatedAt DATETIME DEFAULT GETDATE(),

    CONSTRAINT UQ_Enrollment_Assessment
        UNIQUE(EnrollmentID, AssessmentType),

    CONSTRAINT FK_Scores_Enrollments
        FOREIGN KEY(EnrollmentID)
        REFERENCES Enrollments(EnrollmentID)
        ON DELETE CASCADE
);
GO

/*=========================================================
    10. ATTENDANCE
=========================================================*/

CREATE TABLE Attendance
(
    AttendanceID INT IDENTITY(1,1) PRIMARY KEY,

    EnrollmentID INT NOT NULL,

    AttendanceDate DATE NOT NULL,

    Status NVARCHAR(20)
        CHECK(Status IN ('Present','Absent','Late')),

    CONSTRAINT FK_Attendance_Enrollments
        FOREIGN KEY(EnrollmentID)
        REFERENCES Enrollments(EnrollmentID)
        ON DELETE CASCADE
);
GO

/*=========================================================
    11. TUITION
=========================================================*/

CREATE TABLE Tuition
(
    TuitionID INT IDENTITY(1,1) PRIMARY KEY,

    StudentID NVARCHAR(20) NOT NULL,

    Semester NVARCHAR(30),

    Amount FLOAT NOT NULL,

    PaidAmount FLOAT DEFAULT 0,

    DueDate DATE,

    PaymentStatus NVARCHAR(30)
        DEFAULT 'Unpaid',

    CONSTRAINT FK_Tuition_Students
        FOREIGN KEY(StudentID)
        REFERENCES Students(StudentID)
        ON DELETE CASCADE
);
GO

/*=========================================================
    12. NOTIFICATIONS
=========================================================*/

CREATE TABLE Notifications
(
    NotificationID INT IDENTITY(1,1) PRIMARY KEY,

    UserID NVARCHAR(20) NOT NULL,

    Title NVARCHAR(200) NOT NULL,

    Content NVARCHAR(MAX),

    CreatedAt DATETIME DEFAULT GETDATE(),

    IsRead BIT DEFAULT 0,

    CONSTRAINT FK_Notifications_Users
        FOREIGN KEY(UserID)
        REFERENCES Users(UserID)
        ON DELETE CASCADE
);
GO