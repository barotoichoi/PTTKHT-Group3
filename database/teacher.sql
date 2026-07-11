IF DB_ID('TeacherManagement') IS NULL
    CREATE DATABASE TeacherManagement;
GO

USE TeacherManagement;
GO

IF OBJECT_ID('dbo.Teachers', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Teachers (
        TeacherID NVARCHAR(20) NOT NULL PRIMARY KEY,
        Name NVARCHAR(100) NOT NULL,
        Email NVARCHAR(100) NULL,
        Department NVARCHAR(100) NULL,
        Phone NVARCHAR(20) NULL,
        Username NVARCHAR(50) NULL,
        PasswordHash NVARCHAR(64) NULL,
        Status NVARCHAR(30) NOT NULL DEFAULT 'Active'
    );
END;
GO

IF OBJECT_ID('dbo.TeacherAssignedCourses', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.TeacherAssignedCourses (
        AssignmentID INT IDENTITY(1,1) PRIMARY KEY,
        TeacherID NVARCHAR(20) NOT NULL,
        CourseID NVARCHAR(20) NOT NULL,
        CourseName NVARCHAR(120) NOT NULL,
        Semester NVARCHAR(30) NOT NULL,
        ClassID NVARCHAR(20) NOT NULL,
        CONSTRAINT UQ_TeacherAssignedCourse UNIQUE (TeacherID, CourseID, Semester, ClassID)
    );
END;
GO

IF OBJECT_ID('dbo.TeacherClassStudents', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.TeacherClassStudents (
        ClassStudentID INT IDENTITY(1,1) PRIMARY KEY,
        ClassID NVARCHAR(20) NOT NULL,
        StudentID NVARCHAR(20) NOT NULL,
        Name NVARCHAR(100) NOT NULL,
        Major NVARCHAR(100) NULL,
        Email NVARCHAR(100) NULL,
        CONSTRAINT UQ_TeacherClassStudent UNIQUE (ClassID, StudentID)
    );
END;
GO

IF OBJECT_ID('dbo.TeacherSchedules', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.TeacherSchedules (
        ScheduleID INT IDENTITY(1,1) PRIMARY KEY,
        TeacherID NVARCHAR(20) NOT NULL,
        CourseID NVARCHAR(20) NOT NULL,
        ClassID NVARCHAR(20) NOT NULL,
        DayOfWeek NVARCHAR(20) NOT NULL,
        TimeRange NVARCHAR(30) NOT NULL,
        Room NVARCHAR(30) NULL
    );
END;
GO

IF OBJECT_ID('dbo.TeacherScores', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.TeacherScores (
        ScoreID INT IDENTITY(1,1) PRIMARY KEY,
        TeacherID NVARCHAR(20) NOT NULL,
        CourseID NVARCHAR(20) NOT NULL,
        StudentID NVARCHAR(20) NOT NULL,
        Assessment NVARCHAR(50) NOT NULL,
        Score FLOAT NOT NULL,
        UpdatedAt DATETIME NOT NULL DEFAULT GETDATE(),
        CONSTRAINT UQ_TeacherScore UNIQUE (CourseID, StudentID, Assessment)
    );
END;
GO

IF OBJECT_ID('dbo.TeacherCourseContents', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.TeacherCourseContents (
        ContentID INT IDENTITY(1,1) PRIMARY KEY,
        TeacherID NVARCHAR(20) NOT NULL,
        CourseID NVARCHAR(20) NOT NULL,
        Content NVARCHAR(500) NOT NULL,
        CreatedAt DATETIME NOT NULL DEFAULT GETDATE()
    );
END;
GO

IF OBJECT_ID('dbo.TeacherNotifications', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.TeacherNotifications (
        NotificationID INT IDENTITY(1,1) PRIMARY KEY,
        TeacherID NVARCHAR(20) NULL,
        Message NVARCHAR(500) NOT NULL,
        CreatedAt DATETIME NOT NULL DEFAULT GETDATE(),
        IsRead BIT NOT NULL DEFAULT 0
    );
END;
GO

IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = 'FK_TeacherAssignedCourses_Teachers')
    ALTER TABLE dbo.TeacherAssignedCourses WITH NOCHECK
    ADD CONSTRAINT FK_TeacherAssignedCourses_Teachers
    FOREIGN KEY (TeacherID) REFERENCES dbo.Teachers(TeacherID);
GO

IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = 'FK_TeacherSchedules_Teachers')
    ALTER TABLE dbo.TeacherSchedules WITH NOCHECK
    ADD CONSTRAINT FK_TeacherSchedules_Teachers
    FOREIGN KEY (TeacherID) REFERENCES dbo.Teachers(TeacherID);
GO

IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = 'FK_TeacherScores_Teachers')
    ALTER TABLE dbo.TeacherScores WITH NOCHECK
    ADD CONSTRAINT FK_TeacherScores_Teachers
    FOREIGN KEY (TeacherID) REFERENCES dbo.Teachers(TeacherID);
GO

IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = 'FK_TeacherCourseContents_Teachers')
    ALTER TABLE dbo.TeacherCourseContents WITH NOCHECK
    ADD CONSTRAINT FK_TeacherCourseContents_Teachers
    FOREIGN KEY (TeacherID) REFERENCES dbo.Teachers(TeacherID);
GO

IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = 'FK_TeacherNotifications_Teachers')
    ALTER TABLE dbo.TeacherNotifications WITH NOCHECK
    ADD CONSTRAINT FK_TeacherNotifications_Teachers
    FOREIGN KEY (TeacherID) REFERENCES dbo.Teachers(TeacherID);
GO

MERGE dbo.Teachers AS target
USING (VALUES
    ('TCH001', N'Tran Minh', 'tranminh@edusync.edu.vn', N'Computer Science', '0901000001', 'teacher1', 'f0b96f9d4f2d61a750e6c6a9c2096f6b682a3f0602b7a26b3d0f9b2eb19e3f7a', 'Active'),
    ('TCH002', N'Le Hoa', 'lehoa@edusync.edu.vn', N'Software Engineering', '0901000002', 'teacher2', 'b14d7da907d8b3c0938575ba7b2e5f1ab5f3428228e03b717eb1f5f035e5651b', 'Active')
) AS source (TeacherID,Name,Email,Department,Phone,Username,PasswordHash,Status)
ON target.TeacherID = source.TeacherID
WHEN MATCHED THEN UPDATE SET
    Name = source.Name,
    Email = source.Email,
    Department = source.Department,
    Phone = source.Phone,
    Username = source.Username,
    PasswordHash = source.PasswordHash,
    Status = source.Status
WHEN NOT MATCHED THEN
    INSERT (TeacherID,Name,Email,Department,Phone,Username,PasswordHash,Status)
    VALUES (source.TeacherID,source.Name,source.Email,source.Department,source.Phone,source.Username,source.PasswordHash,source.Status);
GO

MERGE dbo.TeacherAssignedCourses AS target
USING (VALUES
    ('TCH001','CS101',N'Database Systems','2026A','CS101-A'),
    ('TCH001','CS103',N'Computer Networks','2026A','CS103-A'),
    ('TCH002','CS102',N'Object Oriented Programming','2026A','CS102-A'),
    ('TCH002','CS104',N'Web Application Development','2026A','CS104-A')
) AS source (TeacherID,CourseID,CourseName,Semester,ClassID)
ON target.TeacherID = source.TeacherID
   AND target.CourseID = source.CourseID
   AND target.Semester = source.Semester
   AND target.ClassID = source.ClassID
WHEN MATCHED THEN UPDATE SET CourseName = source.CourseName
WHEN NOT MATCHED THEN
    INSERT (TeacherID,CourseID,CourseName,Semester,ClassID)
    VALUES (source.TeacherID,source.CourseID,source.CourseName,source.Semester,source.ClassID);
GO

MERGE dbo.TeacherClassStudents AS target
USING (VALUES
    ('CS101-A','STU001',N'Nguyen Van A',N'Computer Science','a@student.edu.vn'),
    ('CS101-A','STU002',N'Tran Thi B',N'Software Engineering','b@student.edu.vn'),
    ('CS102-A','STU002',N'Tran Thi B',N'Software Engineering','b@student.edu.vn'),
    ('CS103-A','STU003',N'Le Minh C',N'Information Systems','c@student.edu.vn'),
    ('CS104-A','STU004',N'Pham Ngoc D',N'Computer Networks','d@student.edu.vn')
) AS source (ClassID,StudentID,Name,Major,Email)
ON target.ClassID = source.ClassID AND target.StudentID = source.StudentID
WHEN MATCHED THEN UPDATE SET Name=source.Name, Major=source.Major, Email=source.Email
WHEN NOT MATCHED THEN
    INSERT (ClassID,StudentID,Name,Major,Email)
    VALUES (source.ClassID,source.StudentID,source.Name,source.Major,source.Email);
GO

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherSchedules WHERE TeacherID='TCH001' AND CourseID='CS101' AND ClassID='CS101-A')
INSERT INTO dbo.TeacherSchedules (TeacherID,CourseID,ClassID,DayOfWeek,TimeRange,Room)
VALUES ('TCH001','CS101','CS101-A','Monday','07:30-09:30','A101');

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherSchedules WHERE TeacherID='TCH001' AND CourseID='CS103' AND ClassID='CS103-A')
INSERT INTO dbo.TeacherSchedules (TeacherID,CourseID,ClassID,DayOfWeek,TimeRange,Room)
VALUES ('TCH001','CS103','CS103-A','Friday','13:00-15:00','C303');

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherSchedules WHERE TeacherID='TCH002' AND CourseID='CS102' AND ClassID='CS102-A')
INSERT INTO dbo.TeacherSchedules (TeacherID,CourseID,ClassID,DayOfWeek,TimeRange,Room)
VALUES ('TCH002','CS102','CS102-A','Wednesday','09:45-11:45','B202');
GO

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherScores WHERE CourseID='CS101' AND StudentID='STU001' AND Assessment='Midterm')
INSERT INTO dbo.TeacherScores (TeacherID,CourseID,StudentID,Assessment,Score)
VALUES ('TCH001','CS101','STU001','Midterm',7.5);

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherScores WHERE CourseID='CS101' AND StudentID='STU002' AND Assessment='Final')
INSERT INTO dbo.TeacherScores (TeacherID,CourseID,StudentID,Assessment,Score)
VALUES ('TCH001','CS101','STU002','Final',9.0);

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherScores WHERE CourseID='CS102' AND StudentID='STU002' AND Assessment='Assignment')
INSERT INTO dbo.TeacherScores (TeacherID,CourseID,StudentID,Assessment,Score)
VALUES ('TCH002','CS102','STU002','Assignment',8.0);
GO

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherCourseContents WHERE TeacherID='TCH001' AND CourseID='CS101' AND Content=N'Chapter 1: SQL basics')
INSERT INTO dbo.TeacherCourseContents (TeacherID,CourseID,Content)
VALUES ('TCH001','CS101',N'Chapter 1: SQL basics');

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherCourseContents WHERE TeacherID='TCH002' AND CourseID='CS102' AND Content=N'OOP lab exercise 01')
INSERT INTO dbo.TeacherCourseContents (TeacherID,CourseID,Content)
VALUES ('TCH002','CS102',N'OOP lab exercise 01');
GO

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherNotifications WHERE TeacherID='TCH001' AND Message=N'Remember to submit grade reports this week.')
INSERT INTO dbo.TeacherNotifications (TeacherID,Message)
VALUES ('TCH001',N'Remember to submit grade reports this week.');

IF NOT EXISTS (SELECT 1 FROM dbo.TeacherNotifications WHERE TeacherID IS NULL AND Message=N'Faculty meeting at 14:00 on Friday.')
INSERT INTO dbo.TeacherNotifications (TeacherID,Message)
VALUES (NULL,N'Faculty meeting at 14:00 on Friday.');
GO

SELECT 'Teacher seed data is ready.' AS Result;
SELECT TeacherID, Name, Email, Department, Phone, Username, Status FROM dbo.Teachers ORDER BY TeacherID;
SELECT TeacherID, CourseID, CourseName, Semester, ClassID FROM dbo.TeacherAssignedCourses ORDER BY TeacherID, CourseID;
