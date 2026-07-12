-- Tao Data Admin 
INSERT INTO Users
(UserID,Username,Password,Role,Status,FullName,Email,Phone,Gender,DOB)
VALUES
('US001','admin01','admin123','Admin','Active',
N'Nguyễn Văn Quản Lý',
'admin01@school.edu.vn',
'0901234561',
N'Nam',
'1985-05-20');

INSERT INTO Users
(UserID,Username,Password,Role,Status,FullName,Email,Phone,Gender,DOB)
VALUES
('US002','admin02','admin123','Admin','Active',
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
('CS101',N'Database Systems',1,3),
('CS102',N'Object Oriented Programming',2,4),
('CS103',N'Computer Networks',1,3),
('CS104',N'Web Application Development',2,3),
('CS105',N'Data Structures and Algorithms',1,4),
('CS106',N'Artificial Intelligence Concepts',8,3),
('BA201',N'Principles of Marketing',9,3),
('BA202',N'Introduction to Accounting',5,3),
('BA203',N'Corporate Finance',6,3),
('BA204',N'Business Management',10,3);
