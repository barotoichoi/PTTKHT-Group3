USE StudentManagement;
GO

/*=========================================================
    USERS (Teacher)
=========================================================*/

INSERT INTO Users
(
    UserID,
    Username,
    Password,
    Role,
    FullName,
    Email,
    Phone,
    Gender,
    DOB
)
VALUES
('T001','teacher01','123456','Teacher',
N'Nguyen Van An','an@university.edu','0901000001','Male','1985-03-15'),

('T002','teacher02','123456','Teacher',
N'Tran Thi Binh','binh@university.edu','0901000002','Female','1988-07-21'),

('T003','teacher03','123456','Teacher',
N'Le Minh Chau','chau@university.edu','0901000003','Male','1982-11-05');
GO


/*=========================================================
    TEACHERS
=========================================================*/

INSERT INTO Teachers
(
    TeacherID,
    UserID,
    DepartmentID,
    Title
)
VALUES
('GV001','T001',1,N'Lecturer'),
('GV002','T002',2,N'Senior Lecturer'),
('GV003','T003',3,N'Professor');
GOs
/*=========================================================
    COURSES
=========================================================*/

INSERT INTO Courses
(
    CourseID,
    CourseName,
    DepartmentID,
    Credits
)
VALUES
('CS101', N'Programming Fundamentals', 1, 3),
('CS102', N'Database Systems', 1, 3),
('BA101', N'Business Management', 2, 3),
('EN101', N'English Communication', 3, 3);
GO

/*=========================================================
    CLASSES
=========================================================*/

INSERT INTO Classes
(
    ClassID,
    CourseID,
    TeacherID,
    Semester,
    MaxCapacity
)
VALUES
('CLS001','CS101','GV001','Fall 2026',40),
('CLS002','CS102','GV001','Fall 2026',40),
('CLS003','BA101','GV002','Fall 2026',35),
('CLS004','EN101','GV003','Fall 2026',30);
GO


/*=========================================================
    CLASS SCHEDULES
=========================================================*/

INSERT INTO ClassSchedules
(
    ClassID,
    DayOfWeek,
    TimeRange,
    Room
)
VALUES
('CLS001','Monday','07:30-09:30','A101'),
('CLS001','Wednesday','07:30-09:30','A101'),

('CLS002','Tuesday','09:45-11:45','A102'),
('CLS002','Thursday','09:45-11:45','A102'),

('CLS003','Monday','13:00-15:00','B201'),
('CLS003','Friday','13:00-15:00','B201'),

('CLS004','Tuesday','15:15-17:15','C301'),
('CLS004','Thursday','15:15-17:15','C301');
GO
