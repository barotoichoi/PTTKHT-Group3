USE StudentManagement;
GO

-- Tao Data Admin 
INSERT INTO Users
(UserID,Username,Password,Role,Status,FullName,Email,Phone,Gender,DOB)
VALUES
('A001','admin01','admin123','Admin','Active',
N'Nguyễn Văn Quản Lý',
'admin01@school.edu.vn',
'0901234561',
N'Nam',
'1985-05-20');

INSERT INTO Users
(UserID,Username,Password,Role,Status,FullName,Email,Phone,Gender,DOB)
VALUES
('A002','admin02','admin123','Admin','Active',
N'Lê Thị Hoàng Yến',
'admin02@school.edu.vn',
'0901234562',
N'Nữ',
'1988-11-12');

-- Tao Data Department 
INSERT INTO Departments (DepartmentName)
VALUES
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

-- Tao Data Courses
INSERT INTO Courses
(CourseID,CourseName,DepartmentID,Credits)
VALUES
('CS001',N'Database Systems',1,3),
('CS002',N'Object Oriented Programming',2,4),
('CS003',N'Computer Networks',1,3),
('CS004',N'Web Application Development',2,3),
('CS005',N'Data Structures and Algorithms',1,4),
('CS006',N'Artificial Intelligence Concepts',8,3),
('CS007',N'Principles of Marketing',9,3),
('CS008',N'Introduction to Accounting',5,3),
('CS009',N'Corporate Finance',6,3),
('CS010',N'Business Management',10,3);
