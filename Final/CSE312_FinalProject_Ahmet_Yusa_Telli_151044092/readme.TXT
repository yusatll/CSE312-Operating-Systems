Part 2:
Page table size should less than physical memory size. You should be careful to enter framesize and numphysical. When ı find to page table size I use:
page_table_size = (numPhysical * 1024) / frameSize; 
this means that; numphysical is 32K and framesize is 4k then page table size is 32k/4k = 8 entries. If page table is greater than physical memory, the program can be crash.

we have too much access to the page table. therefore, if you enter the number of "pageTablePrintInt" less, you will have trouble tracking. you should enter a larger number like 1 000 000.



Part3:
In this part we do not need numphysical, numvirtual and framesize. You should not enter these numbers. To run program:
$> make
$> ./sortArrays LRU local 10000 diskFileName.dat

Part3.1:
I use c struct for memories and page table. Then take space with malloc. But I can not use free. Because I use malloc with struct(Im not sure). When I use free, prgram give an error.
For this reason,
When I increase framesize I have to stop in 1024. After 1024 frame size running, 2048 framesize has seg fault. But when you give only 2048 framesize it works correctly. 
If I can free memory, it can works. I use 16,32,64,128,256,512,1024 freme size. 

For physical memory, I added index_lru and index_fifo. But I did not use them, then I did not write them in report.

In part3, I do not write page table informations. We need different results.

In disk file you can see -1. It is not mistake. At the beginning, I fill memories to -1. Then I remove from memory I write to disk previous values.



Thank You
Ahmet Yuşa Telli
151044092

note: I think I did the homework well. I'm waiting for a high score. :)


TR:
derdimi anlatamam diye türkçede açıklamak istedim.
part 2:
page table size mantıken physical memory size dan küçük olması lazım. bunun için frameSize verirken biraz büyük bir sayı verilmesi lazım.
page table size ı bulurken:
page_table_size = (numPhysical * 1024) / frameSize; 
kullanıyorum. physical memory 32K olursa 32/4 = 8 olması gerekiyor.
bunun için numphysical ve framesize verirken doğru değerlerin girilmesi gerekir.

part3 çalıştırmak için
$> make
$> ./sortArrays LRU local 10000 diskFileName.dat

part 3.1 de 16,32,64,128,256,512,1024 frame size deniyorum.
benim memory ler struct olduğu için mallocla yer alıyorum ama en son free yapamıyorum. free de hata veriyor. bundan dolayı sonralarda çok yavaş çalışıyor. 2048 ve 4096 için tek çalıştırıldığı zaman çalışıyor. fakat size arttırırken 2048 e gelince hata veriyor. bundan dolayı 2048 i katmadım. 
birde part3.1 de şöyle bir hatayla karşılaştım: program çok yavaşlamıştı frame size 16 da bile bubble sort çok uzun sürüyordu. ama başka bir bilgisayardaki vm makinede denedim o zaman hızlı çalıştı. daha sonra vm kullanan ram arttırdım. normale döndü biraz. böyle bir hatayla karşılaşırsanız ram arttırımı veya başka makinedeki vm de denerseniz güzel olur.

physical memory için farklı index_lru ve index_fifo tutmuştum. fakat kullanmadığım için raporda yazmadım. ama struct içinden de çıkartmadım.

part3 te page table'ı bastırmıyorum. başka sonuçlar öncelikli olduğu için onu kapattım. 


disk dosyasına yazarken bazen -1 ler görebilirsiniz. ben ilk başta hepsini -1le dolduruyorum. ve çıkarttığım page i diske yazıyorum. o nedenle diske -1ler gidiyor. 

not: ödevi güzel yaptığımı düşünüyorum. yüksek bir puan bekliyorum. :)

Teşekkürler
Ahmet Yuşa Telli
151044092