
	VISUAL STUDIO 2013
================================================================
	WIN32 APPLICATION : 1512158 Project
================================================================
Thông tin cá nhân: 
	1512158 - DƯƠNG VĂN MINH HIẾU 
	EMAIL: 	duonghieu121097@gmail.com
================================================================
- CHỨC NĂNG ĐÃ LÀM ĐƯỢC:

   +TreeView:
	1. Tạo node root là This PC
	2. Lấy danh sách các ổ đĩa trong máy bằng hàm GetLogicalDrives hoặc GetLogicalDriveStrings, thêm các ổ đĩa vào node root. 
	3. Lấy ra đường dẫn dấu ở PARAM để biết mình phải xư lí thư mục nào, duyệt nội dung thư mục bằng FindFirstFile & FindNextFile, 
	   chỉ lấy các thư mục để thêm vào làm node con.

   +ListView:
	1. Hiển thị toàn bộ thư mục và tập tin tương ứng với một đường dẫn
	2. Bấm đôi vào một thư mục sẽ thấy toàn bộ thư mục con và tập tin.
	3. Tạo ra ListView có 4 cột: Name, Type, Date modified, Size. Với thư mục và tệp tin set text cho 2 cột Tên và Loại. 

=================================================================
- CÁC LUỒNG SỰ KIỆN CHÍNH:
	
	1. Chạy chương trình lên, hiển thị node This PC trên TreeView bên trái ở trạng thái collapse (thu gọn). Bấm vào sẽ xổ xuống các node con là danh sách ổ đĩa.
	2. Bấm vào ổ đĩa C đang ở trạng thái collapse(thu gọn) trong TreeView bên trái sẽ xổ xuống danh sách các thư mục con.
	3. Bấm vào ổ đĩa hoặc thư mục con trong TreeView bên trái sẽ hiện ra thư mục và tệp tin trong ListView bên phải.
	4. Bấm đôi vào thư mục trong ListView bên phải sẽ hiển thị toàn bộ tập tin và thư mục con.

- CÁC LUỒNG SỰ KIỆN PHỤ:

=================================================================
- Link tới repo chứa mã nguồn đã được upload lên bitbucket: https://DuongHieu1210@bitbucket.org/DuongHieu1210/fileexplorer.git

- Link youtube: https://www.youtube.com/watch?v=3aP5jgigEC8&feature=youtu.be