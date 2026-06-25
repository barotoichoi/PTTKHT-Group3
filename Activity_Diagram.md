# Activity Diagram - UML

## 1. Luồng 1 - Đăng nhập & Điều hướng

```mermaid
flowchart TB
    subgraph User[User]
        A([Start]) --> B[Nhập tài khoản]
        B --> C[Nhấn Đăng nhập]
    end

    subgraph System[Hệ thống]
        C --> D{Xác thực tài khoản}
        D -->|Không hợp lệ| E[Hiển thị lỗi đăng nhập]
        E --> B
        D -->|Hợp lệ| F{Phân quyền người dùng}
        F -->|Admin| G[Đi đến trang chủ Admin]
        F -->|Teacher| H[Đi đến trang chủ Teacher]
        F -->|Student| I[Đi đến trang chủ Student]
    end

    G --> Z([End])
    H --> Z
    I --> Z
```

## 2. Luồng 2 - Sinh viên: Đăng ký môn & Đóng học phí

```mermaid
flowchart TB
    subgraph Student[Student]
        S1([Start]) --> S2[Chọn môn học]
        S2 --> S3[Gửi yêu cầu đăng ký]
    end

    subgraph System[Hệ thống]
        S3 --> T1{Kiểm tra điều kiện học phần}
        T1 -->|Thỏa điều kiện| T2[Đăng ký thành công]
        T1 -->|Không thỏa| T3[Thông báo không đủ điều kiện]
        T3 --> S2
        T2 --> T4[Tính học phí môn đăng ký]
        T4 --> S4[Hiển thị học phí]
    end

    subgraph Student2[Student]
        S4 --> S5[Thanh toán học phí]
    end

    subgraph System2[Hệ thống]
        S5 --> T5[Nhận thanh toán]
        T5 --> T6{Thanh toán thành công?}
        T6 -->|Có| T7[Cập nhật trạng thái đăng ký và học phí]
        T6 -->|Không| T8[Thông báo thanh toán thất bại]
        T8 --> S5
        T7 --> Z([End])
    end
```

## 3. Luồng 3 - Giảng viên: Quản lý lớp & Điểm số

```mermaid
flowchart TB
    subgraph Teacher[Teacher]
        G1([Start]) --> G2[Chọn lớp/khóa học]
        G2 --> G3{Chọn nghiệp vụ}
        G3 -->|Tải tài liệu| G4[Upload tài liệu học tập]
        G3 -->|Quản lý điểm| G5[Chọn sinh viên và thao tác điểm]
    end

    subgraph System[Hệ thống]
        G4 --> H1[Lưu tài liệu học tập]
        G5 --> H2{Nhập / Sửa / Xóa điểm}
        H2 -->|Nhập| H3[Lưu điểm mới]
        H2 -->|Sửa| H4[Cập nhật điểm]
        H2 -->|Xóa| H5[Xóa điểm]
        H3 --> H6[Thông báo cập nhật thành công]
        H4 --> H6
        H5 --> H6
        H1 --> H6
        H6 --> Z([End])
    end
```

## 4. Luồng 4 - Admin: Quản lý Đào tạo

```mermaid
flowchart TB
    subgraph Admin[Admin]
        U1([Start]) --> U2[Tạo Khóa học mới]
        U2 --> U3[Gửi dữ liệu khóa học]
        U4[Phân công giảng viên vào môn học]
    end

    subgraph System[Hệ thống]
        U3 --> V1[Lưu Khóa học]
        V1 --> V2[Xác nhận tạo khóa học]
        V2 --> U4
        U4 --> V3[Lưu phân công giảng viên]
        V3 --> V4{Có gửi thông báo?}
        V4 -->|Có| V5[Gửi thông báo cho Giảng viên]
        V4 -->|Có| V6[Gửi thông báo cho Sinh viên]
        V4 -->|Không| V7[Không gửi thông báo]
        V5 --> V8[Hoàn tất xử lý]
        V6 --> V8
        V7 --> V8
        V8 --> Z([End])
    end
```
