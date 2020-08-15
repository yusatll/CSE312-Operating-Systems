ENG:
We have two different main.cpp for part 2 and 3. These parties work separately from each other.
First, you should run part 2 and create a file system data file. Then, copy this file and paste it into part 3 file.

Run part2:
$> make
$> ./exe
$> makeFileSystem 4 400 fileSystem.data


Run part3:
$> make
$> ./exe
enter comment.txt files line by line.

Part2 only create a file system file. You can chance data block size or number of inode.

Part3 has one problem I found. Sometimes When I read file names in root directory it comes with different characters I do not know why it happens. I tested with my name /ahmet/yusa and ahmet added to root directory but when add yusa, it reads ahmetxd from root directory. But it works clearly other tests. I checked /mm/nn/rr , /usr/ysa/file1 and /os/midterm/part there is no problem.


Run the contents of comment.txt file sequentially to make operations worked clearly.


TR:
Bizim 2 partımızda kod var. Part 2 ve 3'ü ben ayrı ayrı implement ettim. Farklı zipler gönderileceğini gördüğüm için böyle yaptım. 
Part2 de sadece file system oluşturuyor 1mb'lık. oluşan dosyayı part 3 klasörünün içine kopyalamalısınız. Ben ikisi içinde kendi fileSystem.data dosyalarımı gönderiyorum. Part2de oluşturduğunuz .data dosyasının ismini part3'ü çalıştırırkende aynı ismi kullanmalısınız. 

Part3 ü çalıştırırken comment.txt dosyasından kopyalayıp terminale yapıştırmalısınız. comment.txt den direk okuma yaparsanız sıkıntı çıkabilir. 

Part3 içine geldiğimizde linuxFile.data ve linuxFile2.data dosyaları var. Ben bunları write ve read için kullanıyorum. linuxFile2.data dosyası boş. onun içine read ederken dolduracağım.

Write yaparken dosyanın içindekileri alıp kendi file sistemimin içine yazıyorum. Read yaparken de kendi file sistemimde bulunan yazıları gelen dosyanın içine yazıyorum. bu linuzfile.data ların içlerinde ne olması gerekiyor orasını tam anlamadım. Ben linuxFile.data altına kendi ismimi yazıp write operasyonunu test ettim. Dosyanın içindekini alıp gerekli data blocka yazıyorum.

Write ve read belki karışık olmuş olabilir. Bunu bir hata olarak kabul etmezseniz sevinirim. Ben bu şekilde anladım. ikisinin yerinin değişmesi çok önemli değildir diye düşünüyorum.

Part3 de MKDİR için bir problem var. Root directory de yazılan yerleri kontrol ederken eğer dosya ismi uzun olursa okurken saçmalayabiliyor. Ben test ederken "/ahmet/yusa" oluşturdum. ahmet'i root altına yazıyor ama yusa'yi yazarken ahmet'i buluyor fakat okurken "ahmetxd" gibi bir değer okuyor. Bu her zaman olmuyor. Genelde uzun dosya isimlerinde oluyor. Kısa dosya isimlerinde oluştururken hata tespit edemedim. /mm/nn/rr denedim sıkıntı çıkmadı. /usr/ysa/file1 ve /os/midterm/part denedim bunlarda da sıkınyı çıkmadı. Siz test ederken de belki hata çıkmayabilir. Comment.txt içindekilerde sıkıntı çıkmıyor. Onlarla test edebilirsiniz.

Part 3 klasörü içindeki oluşturulmuş olan fileSystem.data dosyası, comment.txt dosyası içindeki mkdir operasyonlarının çalıştırılmış halidir. Temiz olanı part2 yi çalıştırarak elde edebilirsiniz. Bu halinin ekran görüntüsünüde ekliyorum. 


part2 ve 3 içinde comment.txt ler var. onların içindekileri kullanarak dosyaları çalıştırabilirsiniz. 

Rmdir için ben sadece son gelen file'ı siliyorum. Nasıl olacağını tam olarak anlayamadım. Böyle yapmayı uygun gördüm. En son file a gidip data block'u ve inode'u sıfırlıyorum. DEL operasyonun bundan farkını tam olarak anlamadım. İkiside aynı işe yarıyor gibi sanki. İkiside aynı şeyi yapıyor sistemde. 


dumpe2fs de ne yapacağımı anlamadım. Biraz açıklama yapılmış fakat bir beklenen çıktı olsa güzel olurdu. Ben free olan inode ve data block sayılarını ve toplamda olanları yazdım. Sonrasında listeledim. Doğru yapıp yapmadığımdan emin değilim. 

birde del ve rmdir ne gibi fark var. bunlarla ilgili de bir örnek veya beklenen çıktılları gösterseydiniz daha anlaşılır olurdu. Bunlarıda anladığım şekilde yaptım. 

Teşekkürler

Ahmet Yuşa Telli
151044092