IF DB_ID('StudentManagement') IS NULL
    CREATE DATABASE StudentManagement;
GO

USE StudentManagement;
GO

IF OBJECT_ID('dbo.Students', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Students (
        StudentID NVARCHAR(20) NOT NULL PRIMARY KEY,
        Name NVARCHAR(100) NOT NULL,
        Gender NVARCHAR(20) NULL,
        DOB DATE NULL,
        ClassID NVARCHAR(20) NULL,
        Email NVARCHAR(100) NULL,
        Phone NVARCHAR(20) NULL,
        Status NVARCHAR(30) NULL,
        Major NVARCHAR(100) NULL,
        Username NVARCHAR(50) NULL,
        PasswordHash NVARCHAR(64) NULL,
        GPA FLOAT NOT NULL CONSTRAINT DF_Students_GPA DEFAULT 0,
        TuitionOwed FLOAT NOT NULL CONSTRAINT DF_Students_TuitionOwed DEFAULT 0
    );
END;
GO

IF COL_LENGTH('dbo.Students', 'Major') IS NULL
    ALTER TABLE dbo.Students ADD Major NVARCHAR(100) NULL;
GO

IF COL_LENGTH('dbo.Students', 'Username') IS NULL
    ALTER TABLE dbo.Students ADD Username NVARCHAR(50) NULL;
GO

IF COL_LENGTH('dbo.Students', 'PasswordHash') IS NULL
    ALTER TABLE dbo.Students ADD PasswordHash NVARCHAR(64) NULL;
GO

IF COL_LENGTH('dbo.Students', 'GPA') IS NULL
    ALTER TABLE dbo.Students ADD GPA FLOAT NOT NULL CONSTRAINT DF_Students_GPA DEFAULT 0;
GO

IF COL_LENGTH('dbo.Students', 'TuitionOwed') IS NULL
    ALTER TABLE dbo.Students ADD TuitionOwed FLOAT NOT NULL CONSTRAINT DF_Students_TuitionOwed DEFAULT 0;
GO

IF COL_LENGTH('dbo.Students', 'Password') IS NOT NULL
    EXEC('UPDATE dbo.Students SET [Password] = NULL;');
GO

IF OBJECT_ID('dbo.Courses', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Courses (
        CourseID NVARCHAR(20) NOT NULL PRIMARY KEY,
        CourseName NVARCHAR(120) NOT NULL,
        Credits INT NOT NULL,
        TeacherName NVARCHAR(100) NULL,
        Room NVARCHAR(30) NULL,
        Semester NVARCHAR(30) NOT NULL,
        Status NVARCHAR(30) NOT NULL DEFAULT 'Open'
    );
END;
GO

IF OBJECT_ID('dbo.Schedules', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Schedules (
        ScheduleID NVARCHAR(20) NOT NULL PRIMARY KEY,
        CourseID NVARCHAR(20) NOT NULL,
        DayOfWeek NVARCHAR(20) NOT NULL,
        StartTime NVARCHAR(10) NOT NULL,
        EndTime NVARCHAR(10) NOT NULL,
        Room NVARCHAR(30) NULL,
        Semester NVARCHAR(30) NOT NULL
    );
END;
GO

IF OBJECT_ID('dbo.Enrollments', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Enrollments (
        EnrollmentID INT IDENTITY(1,1) PRIMARY KEY,
        StudentID NVARCHAR(20) NOT NULL,
        CourseID NVARCHAR(20) NOT NULL,
        Semester NVARCHAR(30) NOT NULL,
        Status NVARCHAR(30) NOT NULL DEFAULT 'Enrolled',
        CreatedAt DATETIME NOT NULL DEFAULT GETDATE(),
        CONSTRAINT UQ_StudentCourseSemester UNIQUE(StudentID, CourseID, Semester)
    );
END;
GO

IF OBJECT_ID('dbo.Scores', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Scores (
        ScoreID INT IDENTITY(1,1) PRIMARY KEY,
        StudentID NVARCHAR(20) NOT NULL,
        CourseID NVARCHAR(20) NOT NULL,
        Semester NVARCHAR(30) NOT NULL,
        Assignment FLOAT NOT NULL DEFAULT 0,
        Midterm FLOAT NOT NULL DEFAULT 0,
        Final FLOAT NOT NULL DEFAULT 0,
        Average FLOAT NOT NULL DEFAULT 0,
        Grade NVARCHAR(5) NULL,
        CONSTRAINT UQ_StudentScore UNIQUE(StudentID, CourseID, Semester)
    );
END;
GO

IF OBJECT_ID('dbo.CourseMaterials', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.CourseMaterials (
        MaterialID NVARCHAR(20) NOT NULL PRIMARY KEY,
        CourseID NVARCHAR(20) NOT NULL,
        Title NVARCHAR(150) NOT NULL,
        Url NVARCHAR(255) NULL,
        CreatedAt DATETIME NOT NULL DEFAULT GETDATE()
    );
END;
GO

IF OBJECT_ID('dbo.Tuitions', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Tuitions (
        TuitionID NVARCHAR(20) NOT NULL PRIMARY KEY,
        StudentID NVARCHAR(20) NOT NULL,
        Semester NVARCHAR(30) NOT NULL,
        TotalCredits INT NOT NULL,
        Amount FLOAT NOT NULL,
        PaidAmount FLOAT NOT NULL DEFAULT 0,
        DueDate DATE NULL,
        Status NVARCHAR(30) NOT NULL DEFAULT 'Unpaid'
    );
END;
GO

IF OBJECT_ID('dbo.Payments', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Payments (
        PaymentID INT IDENTITY(1,1) PRIMARY KEY,
        TuitionID NVARCHAR(20) NOT NULL,
        Amount FLOAT NOT NULL,
        PaidAt DATETIME NOT NULL DEFAULT GETDATE(),
        Method NVARCHAR(30) NOT NULL DEFAULT 'Cash'
    );
END;
GO

IF OBJECT_ID('dbo.Notifications', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Notifications (
        NotificationID NVARCHAR(20) NOT NULL PRIMARY KEY,
        StudentID NVARCHAR(20) NULL,
        Title NVARCHAR(120) NOT NULL,
        Content NVARCHAR(500) NOT NULL,
        CreatedAt DATETIME NOT NULL DEFAULT GETDATE(),
        IsRead BIT NOT NULL DEFAULT 0,
        Type NVARCHAR(30) NOT NULL DEFAULT 'General'
    );
END;
GO

MERGE dbo.Students AS target
USING (VALUES
    ('STU001',N'Nguyen Van A','Male','2005-01-01','CS101','a@student.edu.vn','0912345678','Active',N'Computer Science','student1','4e4fc8eb3ef5bdc603e8ea370154cf2f58255d5b18319edd3d614bd4fac8dea9',3.3,3500000),
    ('STU002',N'Tran Thi B','Female','2005-03-12','CS101','b@student.edu.vn','0912345679','Active',N'Software Engineering','student2','86947df1353a68bd967f8b7f403509c0ee3d0212f8641a92c4d05177292e5f80',3.6,4200000),
    ('STU003',N'Le Minh C','Male','2004-11-22','CS102','c@student.edu.vn','0912345680','Warning',N'Information Systems','student3','9513e36755cce9271fe55bb728570347f471c710a142e4c86d75ff279992cbd8',2.9,2500000),
    ('STU004',N'Pham Ngoc D','Female','2005-07-19','CS103','d@student.edu.vn','0912345681','Active',N'Computer Networks','student4','8c220ca4fb36860dd074b7da0b24f175cb42399d36a5c91e7be6d348f740212f',3.1,0),
    ('STU005',N'Hoang Gia E','Male','2005-09-30','CS104','e@student.edu.vn','0912345682','Active',N'Data Science','student5','2b2b160f25bb6d090cbc0517457f9962b8ea1174ece69ae076c654857ec125d5',3.8,1800000)
) AS source (StudentID,Name,Gender,DOB,ClassID,Email,Phone,Status,Major,Username,PasswordHash,GPA,TuitionOwed)
ON target.StudentID = source.StudentID
WHEN MATCHED THEN UPDATE SET
    Name = source.Name,
    Gender = source.Gender,
    DOB = source.DOB,
    ClassID = source.ClassID,
    Email = source.Email,
    Phone = source.Phone,
    Status = source.Status,
    Major = source.Major,
    Username = source.Username,
    PasswordHash = source.PasswordHash,
    GPA = source.GPA,
    TuitionOwed = source.TuitionOwed
WHEN NOT MATCHED THEN
    INSERT (StudentID,Name,Gender,DOB,ClassID,Email,Phone,Status,Major,Username,PasswordHash,GPA,TuitionOwed)
    VALUES (source.StudentID,source.Name,source.Gender,source.DOB,source.ClassID,source.Email,source.Phone,source.Status,source.Major,source.Username,source.PasswordHash,source.GPA,source.TuitionOwed);
GO

MERGE dbo.Courses AS target
USING (VALUES
    ('CS101',N'Database Systems',3,N'Tran Minh','A101','2026A','Open'),
    ('CS102',N'Object Oriented Programming',3,N'Le Hoa','B202','2026A','Open'),
    ('CS103',N'Computer Networks',2,N'Pham Long','C303','2026A','Open'),
    ('CS104',N'Web Application Development',3,N'Nguyen Son','D404','2026A','Open'),
    ('CS105',N'Data Structures and Algorithms',4,N'Do Anh','E505','2026A','Open'),
    ('CS201',N'Artificial Intelligence',3,N'Vu Linh','F606','2026B','Open'),
    ('CS202',N'Software Testing',2,N'Bui Ha','G707','2026B','Open')
) AS source (CourseID,CourseName,Credits,TeacherName,Room,Semester,Status)
ON target.CourseID = source.CourseID
WHEN MATCHED THEN UPDATE SET CourseName=source.CourseName, Credits=source.Credits, TeacherName=source.TeacherName, Room=source.Room, Semester=source.Semester, Status=source.Status
WHEN NOT MATCHED THEN INSERT (CourseID,CourseName,Credits,TeacherName,Room,Semester,Status)
VALUES (source.CourseID,source.CourseName,source.Credits,source.TeacherName,source.Room,source.Semester,source.Status);
GO

MERGE dbo.Schedules AS target
USING (VALUES
    ('SCH101','CS101','Monday','07:30','09:30','A101','2026A'),
    ('SCH102','CS102','Wednesday','09:45','11:45','B202','2026A'),
    ('SCH103','CS103','Friday','13:00','15:00','C303','2026A'),
    ('SCH104','CS104','Tuesday','07:30','09:30','D404','2026A'),
    ('SCH105','CS105','Monday','09:45','12:00','E505','2026A'),
    ('SCH201','CS201','Thursday','07:30','09:30','F606','2026B'),
    ('SCH202','CS202','Friday','09:45','11:45','G707','2026B')
) AS source (ScheduleID,CourseID,DayOfWeek,StartTime,EndTime,Room,Semester)
ON target.ScheduleID = source.ScheduleID
WHEN MATCHED THEN UPDATE SET CourseID=source.CourseID, DayOfWeek=source.DayOfWeek, StartTime=source.StartTime, EndTime=source.EndTime, Room=source.Room, Semester=source.Semester
WHEN NOT MATCHED THEN INSERT (ScheduleID,CourseID,DayOfWeek,StartTime,EndTime,Room,Semester)
VALUES (source.ScheduleID,source.CourseID,source.DayOfWeek,source.StartTime,source.EndTime,source.Room,source.Semester);
GO

IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU001' AND CourseID='CS101' AND Semester='2026A') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU001','CS101','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU001' AND CourseID='CS105' AND Semester='2026A') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU001','CS105','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU002' AND CourseID='CS101' AND Semester='2026A') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU002','CS101','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU002' AND CourseID='CS102' AND Semester='2026A') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU002','CS102','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU003' AND CourseID='CS103' AND Semester='2026A') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU003','CS103','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU004' AND CourseID='CS104' AND Semester='2026A') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU004','CS104','2026A','Enrolled');
GO

IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU001' AND CourseID='CS101') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU001','CS101','2026A',8,7.5,8.5,8.1,'A');
IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU001' AND CourseID='CS105') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU001','CS105','2026A',7,8,7.5,7.5,'B');
IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU002' AND CourseID='CS101') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU002','CS101','2026A',9,8.5,9,8.85,'A');
IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU002' AND CourseID='CS102') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU002','CS102','2026A',8,8,8,8,'A');
IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU003' AND CourseID='CS103') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU003','CS103','2026A',6,5.5,6.5,6.1,'C');
GO

IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT101') INSERT INTO dbo.CourseMaterials (MaterialID,CourseID,Title,Url,CreatedAt) VALUES ('MAT101','CS101',N'Lecture 01 - SQL Basics','https://school.local/cs101/lecture01',GETDATE());
IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT102') INSERT INTO dbo.CourseMaterials (MaterialID,CourseID,Title,Url,CreatedAt) VALUES ('MAT102','CS102',N'OOP Lab Guide','https://school.local/cs102/lab',GETDATE());
IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT103') INSERT INTO dbo.CourseMaterials (MaterialID,CourseID,Title,Url,CreatedAt) VALUES ('MAT103','CS103',N'Network Simulation Packet Tracer','https://school.local/cs103/packet-tracer',GETDATE());
IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT104') INSERT INTO dbo.CourseMaterials (MaterialID,CourseID,Title,Url,CreatedAt) VALUES ('MAT104','CS104',N'HTML CSS JavaScript Starter','https://school.local/cs104/web-starter',GETDATE());
IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT105') INSERT INTO dbo.CourseMaterials (MaterialID,CourseID,Title,Url,CreatedAt) VALUES ('MAT105','CS105',N'Algorithm Practice Set','https://school.local/cs105/practice',GETDATE());
GO

IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI001') INSERT INTO dbo.Tuitions (TuitionID,StudentID,Semester,TotalCredits,Amount,PaidAmount,DueDate,Status) VALUES ('TUI001','STU001','2026A',7,3500000,0,'2026-08-15','Unpaid');
IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI002') INSERT INTO dbo.Tuitions (TuitionID,StudentID,Semester,TotalCredits,Amount,PaidAmount,DueDate,Status) VALUES ('TUI002','STU002','2026A',6,3000000,1500000,'2026-08-15','Partial');
IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI003') INSERT INTO dbo.Tuitions (TuitionID,StudentID,Semester,TotalCredits,Amount,PaidAmount,DueDate,Status) VALUES ('TUI003','STU003','2026A',2,1000000,0,'2026-08-20','Unpaid');
IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI004') INSERT INTO dbo.Tuitions (TuitionID,StudentID,Semester,TotalCredits,Amount,PaidAmount,DueDate,Status) VALUES ('TUI004','STU004','2026A',3,1500000,1500000,'2026-08-20','Paid');
IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI005') INSERT INTO dbo.Tuitions (TuitionID,StudentID,Semester,TotalCredits,Amount,PaidAmount,DueDate,Status) VALUES ('TUI005','STU005','2026A',0,0,0,'2026-08-20','Paid');
GO

IF NOT EXISTS (SELECT 1 FROM dbo.Payments WHERE TuitionID='TUI002') INSERT INTO dbo.Payments (TuitionID,Amount,Method) VALUES ('TUI002',1500000,'BankTransfer');
IF NOT EXISTS (SELECT 1 FROM dbo.Payments WHERE TuitionID='TUI004') INSERT INTO dbo.Payments (TuitionID,Amount,Method) VALUES ('TUI004',1500000,'Cash');
GO

IF NOT EXISTS (SELECT 1 FROM dbo.Notifications WHERE NotificationID='NOTI001') INSERT INTO dbo.Notifications (NotificationID,StudentID,Title,Content,CreatedAt,IsRead,Type) VALUES ('NOTI001','STU001',N'Tuition deadline',N'Please pay tuition for semester 2026A before due date.',GETDATE(),0,'Tuition');
IF NOT EXISTS (SELECT 1 FROM dbo.Notifications WHERE NotificationID='NOTI002') INSERT INTO dbo.Notifications (NotificationID,StudentID,Title,Content,CreatedAt,IsRead,Type) VALUES ('NOTI002','STU002',N'Partial tuition payment',N'Your tuition payment is partially completed.',GETDATE(),0,'Tuition');
IF NOT EXISTS (SELECT 1 FROM dbo.Notifications WHERE NotificationID='NOTI003') INSERT INTO dbo.Notifications (NotificationID,StudentID,Title,Content,CreatedAt,IsRead,Type) VALUES ('NOTI003',NULL,N'New course opened',N'AI and Software Testing are open for 2026B registration.',GETDATE(),0,'General');
IF NOT EXISTS (SELECT 1 FROM dbo.Notifications WHERE NotificationID='NOTI004') INSERT INTO dbo.Notifications (NotificationID,StudentID,Title,Content,CreatedAt,IsRead,Type) VALUES ('NOTI004','STU003',N'Academic warning',N'Please meet your advisor about academic progress.',GETDATE(),0,'General');
GO

SELECT 'Student seed data is ready.' AS Result;
SELECT StudentID, Name, Major, Username, PasswordHash, GPA, TuitionOwed FROM dbo.Students ORDER BY StudentID;
SELECT CourseID, CourseName, Credits, TeacherName, Room, Semester, Status FROM dbo.Courses ORDER BY CourseID;
