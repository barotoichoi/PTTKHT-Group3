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
N'Le Minh Cuong','cuong@university.edu','0901000003','Male','1983-11-10'),

('T004','teacher04','123456','Teacher',
N'Pham Thu Ha','ha@university.edu','0901000004','Female','1990-01-08'),

('T005','teacher05','123456','Teacher',
N'Vo Quoc Hung','hung@university.edu','0901000005','Male','1987-09-17');

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
('T001','T001',1,N'Lecturer'),
('T002','T002',1,N'Senior Lecturer'),
('T003','T003',2,N'Lecturer'),
('T004','T004',3,N'Lecturer'),
('T005','T005',5,N'Associate Professor');

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
('CLS001','CRS00001','T001','Fall 2026',40),
('CLS002','CRS00002','T002','Fall 2026',40),
('CLS003','CRS00003','T003','Fall 2026',35),
('CLS004','CRS00004','T004','Fall 2026',45),
('CLS005','CRS00005','T005','Fall 2026',40);

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
('CLS001',N'Monday','07:30 - 09:30','A101'),

('CLS001',N'Wednesday','07:30 - 09:30','A101'),

('CLS002',N'Tuesday','09:45 - 11:45','B202'),

('CLS002',N'Thursday','09:45 - 11:45','B202'),

('CLS003',N'Monday','13:00 - 15:00','C301'),

('CLS004',N'Friday','07:30 - 10:30','D401'),

('CLS005',N'Saturday','09:00 - 12:00','E501');