# NP_hw1
[題目要求]
請實作一個小型的web server，具有底下功能：

1. 可從一般的瀏覽器，例如Chrome，取得所設計之web server上的網頁。

2. 網頁中需至少有一張圖片，並正確在瀏覽器上顯示。

3. 可以從瀏覽器上上傳檔案，並正確儲存至web server上。


技術細節：

1. 此web server必須為concurrent server。

2. 必須不可留下zombie process。


[程式流程]

1.main function部分

 (1)開啟 Socket
 
 (2)設定網路連線 (TCP/IP協定、 port、對外 IP)
 
 (3)開啟監聽器 bind()
 
 (4)監聽 listen()
 
 (5)等待客戶端連線 accept()
 
 (6)若有客戶連線 fork()出子行程
 
 (8)子行程呼叫handle_socketf處理瀏覽器要求
 
   (8.1)接收瀏覽器傳來的buffer，初步分析其中的訊息
   
   (8.2)如果是GET則讀取需要的檔案以標準的格式回傳給瀏覽器
   
   (8.3)如果是post就用字串處理找出檔案的名稱和內容，開檔寫檔將檔案存在wed server上
   
 (9)關掉父行程
 
 
 [期望加分]
 可以看在用網頁寫了一個可愛的小遊戲很用心的份上加點分數嗎QAQ


[參考資料]

http://fred-zone.blogspot.com/2007/09/http-web-server.html

https://notfalse.net/44/http-get-vs-post

https://github.com/PacktPublishing/Hands-On-Network-Programming-with-C/blob/master/chap07/web_server2.c
