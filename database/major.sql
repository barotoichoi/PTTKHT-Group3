IF DB_ID('StudentManagement') IS NULL
    CREATE DATABASE StudentManagement;
GO

USE StudentManagement;
GO

IF OBJECT_ID('dbo.Majors', 'U') IS NULL
BEGIN
    CREATE TABLE dbo.Majors (
        MajorID NVARCHAR(20) NOT NULL PRIMARY KEY,
        Name NVARCHAR(100) NOT NULL,
        Description NVARCHAR(255) NULL,
        Status NVARCHAR(30) NOT NULL DEFAULT 'Active'
    );
END;
GO

INSERT INTO dbo.Majors (MajorID, Name, Description, Status) VALUES
(N'MJR001', N'Công nghệ thông tin', N'Công nghệ thông tin description', 'Active'),
(N'MJR002', N'Kinh tế', N'Kinh tế description', 'Active'),
(N'MJR003', N'Cơ khí', N'Cơ khí description', 'Active'),
(N'MJR004', N'Điện tử - Viễn thông', N'Điện tử - Viễn thông description', 'Active'),
(N'MJR005', N'Quản trị kinh doanh', N'Quản trị kinh doanh description', 'Active'),
(N'MJR006', N'Marketing', N'Marketing description', 'Active'),
(N'MJR007', N'Tài chính - Ngân hàng', N'Tài chính - Ngân hàng description', 'Active'),
(N'MJR008', N'Luật', N'Luật description', 'Active'),
(N'MJR009', N'Ngôn ngữ Anh', N'Ngôn ngữ Anh description', 'Active'),
(N'MJR010', N'Công nghệ ô tô', N'Công nghệ ô tô description', 'Active'),
(N'MJR011', N'An toàn thông tin', N'An toàn thông tin description', 'Active'),
(N'MJR012', N'Kỹ thuật máy tính', N'Kỹ thuật máy tính description', 'Active'),
(N'MJR013', N'Kiến trúc', N'Kiến trúc description', 'Active'),
(N'MJR014', N'Quản lý xây dựng', N'Quản lý xây dựng description', 'Active'),
(N'MJR015', N'Công nghệ sinh học', N'Công nghệ sinh học description', 'Active');