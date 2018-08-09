# c_and_python_share_mem
# stb_restful
Restful API server with IPC between python and C

這個範例程式用 Share memory 實作 C 和 Python 之間的資料交換。
Share memory Id 10000 是 C to Python, Id 10001 是 Python to C。

1. Build test_stb.cpp and run 
gcc stb_test.cpp queue.cpp -o stb_test
這支程式會開兩 threads, 一個 read queue, 一個 write queue
read queue thread 會一直從 share memory id 10001 讀資料，
讀到後就印出來，然後清掉資料。
write queue thread 則是從 stdin 讀取輸入的資料後，送到 share memory id 10000

2. 執行 stb_rest.py  Python 程式，並帶 http port 當參數
stb_rest 會用 Flask 套件啟動 web server，port 就是 argv[1]

3. 用瀏覽器連到執行 stb_rest 的主機，port 要輸入 2. 的 http port
url 後面接 /set_data/<str> 後，瀏覽器會印出 Done，而test_stb 程式視窗會印出 str 內容。
url 後面接 /get_data，瀏覽器會印出剛才 test_stb 輸入的內容。


#20170905 修改成為單一c++和和c檔
因為stb主程式是c格式,所以將 stb_restful c++格式改為c格式以方便加入stb 主程式
使用方式和原本一樣
python2 run ok
python3 set_data ok but get_data NG
(pi 編譯===>gcc stb_test.c -o stb_test.a -lpthread)
install pip3
sudo apt-get install python3-pip
install sysv-ipc
sudo apt-get install python3-sysv-ipc
