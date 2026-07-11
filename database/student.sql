USE StudentManagement;
GO

IF COL_LENGTH('dbo.Students', 'Major') IS NULL
    ALTER TABLE dbo.Students ADD Major NVARCHAR(100) NULL;
GO

IF COL_LENGTH('dbo.Students', 'Username') IS NULL
    ALTER TABLE dbo.Students ADD Username NVARCHAR(50) NULL;
GO

IF COL_LENGTH('dbo.Students', 'Password') IS NULL
    ALTER TABLE dbo.Students ADD [Password] NVARCHAR(100) NULL;
GO

IF COL_LENGTH('dbo.Students', 'GPA') IS NULL
    ALTER TABLE dbo.Students ADD GPA FLOAT NOT NULL CONSTRAINT DF_Students_GPA DEFAULT 0;
GO

IF COL_LENGTH('dbo.Students', 'TuitionOwed') IS NULL
    ALTER TABLE dbo.Students ADD TuitionOwed FLOAT NOT NULL CONSTRAINT DF_Students_TuitionOwed DEFAULT 0;
GO

IF NOT EXISTS (SELECT 1 FROM dbo.Students WHERE StudentID = 'STU001')
INSERT INTO dbo.Students (StudentID,Name,Gender,DOB,ClassID,Email,Major,Phone,Username,[Password],GPA,TuitionOwed,Status)
VALUES ('STU001',N'Nguyen Van A','Male','2005-01-01','CS101','a@student.edu.vn',N'Computer Science','0912345678','student1','123456',3.3,3500000,'Active');

IF NOT EXISTS (SELECT 1 FROM dbo.Students WHERE StudentID = 'STU002')
INSERT INTO dbo.Students (StudentID,Name,Gender,DOB,ClassID,Email,Major,Phone,Username,[Password],GPA,TuitionOwed,Status)
VALUES ('STU002',N'Tran Thi B','Female','2005-03-12','CS101','b@student.edu.vn',N'Software Engineering','0912345679','student2','123456',3.6,4200000,'Active');

IF NOT EXISTS (SELECT 1 FROM dbo.Students WHERE StudentID = 'STU003')
INSERT INTO dbo.Students (StudentID,Name,Gender,DOB,ClassID,Email,Major,Phone,Username,[Password],GPA,TuitionOwed,Status)
VALUES ('STU003',N'Le Minh C','Male','2004-11-22','CS102','c@student.edu.vn',N'Information Systems','0912345680','student3','123456',2.9,2500000,'Warning');

IF NOT EXISTS (SELECT 1 FROM dbo.Students WHERE StudentID = 'STU004')
INSERT INTO dbo.Students (StudentID,Name,Gender,DOB,ClassID,Email,Major,Phone,Username,[Password],GPA,TuitionOwed,Status)
VALUES ('STU004',N'Pham Ngoc D','Female','2005-07-19','CS103','d@student.edu.vn',N'Computer Networks','0912345681','student4','123456',3.1,0,'Active');

IF NOT EXISTS (SELECT 1 FROM dbo.Students WHERE StudentID = 'STU005')
INSERT INTO dbo.Students (StudentID,Name,Gender,DOB,ClassID,Email,Major,Phone,Username,[Password],GPA,TuitionOwed,Status)
VALUES ('STU005',N'Hoang Gia E','Male','2005-09-30','CS104','e@student.edu.vn',N'Data Science','0912345682','student5','123456',3.8,1800000,'Active');

IF NOT EXISTS (SELECT 1 FROM dbo.Courses WHERE CourseID = 'CS101') INSERT INTO dbo.Courses VALUES ('CS101',N'Database Systems',3,N'Tran Minh','A101','2026A','Open');
IF NOT EXISTS (SELECT 1 FROM dbo.Courses WHERE CourseID = 'CS102') INSERT INTO dbo.Courses VALUES ('CS102',N'Object Oriented Programming',3,N'Le Hoa','B202','2026A','Open');
IF NOT EXISTS (SELECT 1 FROM dbo.Courses WHERE CourseID = 'CS103') INSERT INTO dbo.Courses VALUES ('CS103',N'Computer Networks',2,N'Pham Long','C303','2026A','Open');
IF NOT EXISTS (SELECT 1 FROM dbo.Courses WHERE CourseID = 'CS104') INSERT INTO dbo.Courses VALUES ('CS104',N'Web Application Development',3,N'Nguyen Son','D404','2026A','Open');
IF NOT EXISTS (SELECT 1 FROM dbo.Courses WHERE CourseID = 'CS105') INSERT INTO dbo.Courses VALUES ('CS105',N'Data Structures and Algorithms',4,N'Do Anh','E505','2026A','Open');
IF NOT EXISTS (SELECT 1 FROM dbo.Courses WHERE CourseID = 'CS201') INSERT INTO dbo.Courses VALUES ('CS201',N'Artificial Intelligence',3,N'Vu Linh','F606','2026B','Open');
IF NOT EXISTS (SELECT 1 FROM dbo.Courses WHERE CourseID = 'CS202') INSERT INTO dbo.Courses VALUES ('CS202',N'Software Testing',2,N'Bui Ha','G707','2026B','Open');

IF NOT EXISTS (SELECT 1 FROM dbo.Schedules WHERE ScheduleID = 'SCH101') INSERT INTO dbo.Schedules VALUES ('SCH101','CS101','Monday','07:30','09:30','A101','2026A');
IF NOT EXISTS (SELECT 1 FROM dbo.Schedules WHERE ScheduleID = 'SCH102') INSERT INTO dbo.Schedules VALUES ('SCH102','CS102','Wednesday','09:45','11:45','B202','2026A');
IF NOT EXISTS (SELECT 1 FROM dbo.Schedules WHERE ScheduleID = 'SCH103') INSERT INTO dbo.Schedules VALUES ('SCH103','CS103','Friday','13:00','15:00','C303','2026A');
IF NOT EXISTS (SELECT 1 FROM dbo.Schedules WHERE ScheduleID = 'SCH104') INSERT INTO dbo.Schedules VALUES ('SCH104','CS104','Tuesday','07:30','09:30','D404','2026A');
IF NOT EXISTS (SELECT 1 FROM dbo.Schedules WHERE ScheduleID = 'SCH105') INSERT INTO dbo.Schedules VALUES ('SCH105','CS105','Monday','09:45','12:00','E505','2026A');
IF NOT EXISTS (SELECT 1 FROM dbo.Schedules WHERE ScheduleID = 'SCH201') INSERT INTO dbo.Schedules VALUES ('SCH201','CS201','Thursday','07:30','09:30','F606','2026B');
IF NOT EXISTS (SELECT 1 FROM dbo.Schedules WHERE ScheduleID = 'SCH202') INSERT INTO dbo.Schedules VALUES ('SCH202','CS202','Friday','09:45','11:45','G707','2026B');

IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU001' AND CourseID='CS101') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU001','CS101','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU001' AND CourseID='CS105') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU001','CS105','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU002' AND CourseID='CS101') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU002','CS101','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU002' AND CourseID='CS102') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU002','CS102','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU003' AND CourseID='CS103') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU003','CS103','2026A','Enrolled');
IF NOT EXISTS (SELECT 1 FROM dbo.Enrollments WHERE StudentID='STU004' AND CourseID='CS104') INSERT INTO dbo.Enrollments (StudentID,CourseID,Semester,Status) VALUES ('STU004','CS104','2026A','Enrolled');

IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU001' AND CourseID='CS101') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU001','CS101','2026A',8,7.5,8.5,8.1,'A');
IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU001' AND CourseID='CS105') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU001','CS105','2026A',7,8,7.5,7.5,'B');
IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU002' AND CourseID='CS101') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU002','CS101','2026A',9,8.5,9,8.85,'A');
IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU002' AND CourseID='CS102') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU002','CS102','2026A',8,8,8,8,'A');
IF NOT EXISTS (SELECT 1 FROM dbo.Scores WHERE StudentID='STU003' AND CourseID='CS103') INSERT INTO dbo.Scores (StudentID,CourseID,Semester,Assignment,Midterm,Final,Average,Grade) VALUES ('STU003','CS103','2026A',6,5.5,6.5,6.1,'C');

IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT101') INSERT INTO dbo.CourseMaterials VALUES ('MAT101','CS101',N'Lecture 01 - SQL Basics','https://school.local/cs101/lecture01',GETDATE());
IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT102') INSERT INTO dbo.CourseMaterials VALUES ('MAT102','CS102',N'OOP Lab Guide','https://school.local/cs102/lab',GETDATE());
IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT103') INSERT INTO dbo.CourseMaterials VALUES ('MAT103','CS103',N'Network Simulation Packet Tracer','https://school.local/cs103/packet-tracer',GETDATE());
IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT104') INSERT INTO dbo.CourseMaterials VALUES ('MAT104','CS104',N'HTML CSS JavaScript Starter','https://school.local/cs104/web-starter',GETDATE());
IF NOT EXISTS (SELECT 1 FROM dbo.CourseMaterials WHERE MaterialID='MAT105') INSERT INTO dbo.CourseMaterials VALUES ('MAT105','CS105',N'Algorithm Practice Set','https://school.local/cs105/practice',GETDATE());

IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI001') INSERT INTO dbo.Tuitions VALUES ('TUI001','STU001','2026A',7,3500000,0,'2026-08-15','Unpaid');
IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI002') INSERT INTO dbo.Tuitions VALUES ('TUI002','STU002','2026A',6,3000000,1500000,'2026-08-15','Partial');
IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI003') INSERT INTO dbo.Tuitions VALUES ('TUI003','STU003','2026A',2,1000000,0,'2026-08-20','Unpaid');
IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI004') INSERT INTO dbo.Tuitions VALUES ('TUI004','STU004','2026A',3,1500000,1500000,'2026-08-20','Paid');
IF NOT EXISTS (SELECT 1 FROM dbo.Tuitions WHERE TuitionID='TUI005') INSERT INTO dbo.Tuitions VALUES ('TUI005','STU005','2026A',0,0,0,'2026-08-20','Paid');

IF NOT EXISTS (SELECT 1 FROM dbo.Payments WHERE TuitionID='TUI002') INSERT INTO dbo.Payments (TuitionID,Amount,Method) VALUES ('TUI002',1500000,'BankTransfer');
IF NOT EXISTS (SELECT 1 FROM dbo.Payments WHERE TuitionID='TUI004') INSERT INTO dbo.Payments (TuitionID,Amount,Method) VALUES ('TUI004',1500000,'Cash');

IF NOT EXISTS (SELECT 1 FROM dbo.Notifications WHERE NotificationID='NOTI001') INSERT INTO dbo.Notifications VALUES ('NOTI001','STU001',N'Tuition deadline',N'Please pay tuition for semester 2026A before due date.',GETDATE(),0,'Tuition');
IF NOT EXISTS (SELECT 1 FROM dbo.Notifications WHERE NotificationID='NOTI002') INSERT INTO dbo.Notifications VALUES ('NOTI002','STU002',N'Partial tuition payment',N'Your tuition payment is partially completed.',GETDATE(),0,'Tuition');
IF NOT EXISTS (SELECT 1 FROM dbo.Notifications WHERE NotificationID='NOTI003') INSERT INTO dbo.Notifications VALUES ('NOTI003',NULL,N'New course opened',N'AI and Software Testing are open for 2026B registration.',GETDATE(),0,'General');
IF NOT EXISTS (SELECT 1 FROM dbo.Notifications WHERE NotificationID='NOTI004') INSERT INTO dbo.Notifications VALUES ('NOTI004','STU003',N'Academic warning',N'Please meet your advisor about academic progress.',GETDATE(),0,'General');

SELECT 'Student seed data is ready.' AS Result;
