Merhabalar,
Ödevde 3 kısım bulunuyor. 3 tane farklı SPIMOS_GTU dosyası var. 
Bu programları kullanabilmek için kendim özel olarak bir kaç register tuttum.
Bu özel registerlar üzerinden işlemler yaptım. Bu nedenle de 7 tane kendim syscall
oluşturdum. 

Programlarda sıkıntı olabiliyor. Sağlıklı sonuçlar alabilmek için çalıştırma şekli:
> make clean
> sudo make install
> spim read SPIMOS_GTU_1.s
olarak çalıştırılması gerekiyor. PDF te yazan -efexceptions denedim.
Fakat en başlangıçta bile hata aldım. Basit kodları bile çalıştıramadım.
Kullanamadım/kullanmadım. 
Programlar normal olarak çalışıyor. Ama bazen sonsuz döngüde kalabiliyor. 
Memory kullanımıdan olduğunu düşünüyorum. Ama 5 denemeden 3-4 ünde çalışıyor diyebilirim. 

Programların aralarına pdf te belirtildiği gibi register değerlerini bastırdım. 
O nedenle programların çıktıları biraz aralarda kalabiliyor. 
Ekran görüntülerinde sadece registerların ekrana bastırılma yerini yoruma aldım. 
Daha kolay görünmesi için. Aralara registerları da yazdırıyorum. 

Ufak bir hatadan dolayı bazen sonsuz döngüde kalabiliyor. Ama diğer programlar
binary search, linear search, collatz sorunsuz çalışıyor. Onların içinde kendi 
oluşturduğum syscall ları kullandığım için tek olarak çalıştırabilmek için bir iki 
ufak değişiklik yapmak gerekiyor. Mesela exit için 10 yerine ben 20 kullanıyorum.

Bu bilgilerin notumun değerlendirilmesinde size kolaylık sunacağını umuyorum.
Teşekkürler

Ahmet Yuşa Telli
151044092