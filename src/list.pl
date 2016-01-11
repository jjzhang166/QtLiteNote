use 5.010;
use warnings;
use strict;
use CharsetDetector;
use CharsetDetector qw(detect detect1);

sub main;
sub isUTF8;
sub isGBK;
sub listFiles;

main;

sub main {
    listFiles;
}

sub listFiles {

    opendir(DIR, ".");
    my @files = readdir(DIR);

    my $maxLen = 0;
    my %fileTypes;

    foreach (@files) {
        my $f = $_;
        my $len = length($f);
        if ($len > $maxLen) {
            $maxLen = $len;
        }

        if (-T $_) {
            if (isUTF8($f)) {
                $fileTypes{$f} = "UTF8";
            } elsif (isGBK($f)) {
                $fileTypes{$f} = "GBK";
            } else {
                $fileTypes{$f} = "Unkown";
            }
        }
    }
    $maxLen = $maxLen + 8;

    # 数字的三向比较操作符是 <=> 
    # 字符串是用 cmp
    my @keys = sort{$fileTypes{$a} cmp $fileTypes{$b}} keys %fileTypes;

    foreach (@keys) {
        my $k = $_;
        my $v = $fileTypes{$k};

        #printf "%-20s  %s", "first", "second"
        #第一个字符串占20个位置，并且靠左显示
        printf "%-$maxLen"."s $v\n", $k;
    }

}

sub isFileChset {
    my $file = shift;
    my $chs = shift;
    my %chset;

    open(FILE, "$file");

    while (<FILE>) {
        my $ch = CharsetDetector::detect($_);
        $chset{$ch} = 0;
    }
    close FILE;

    if (exists $chset{$chs}) {
        return 1;
    }
    return 0;
}

# 某些都是英文的文件，每行都会是iso-8859-1编码，而这时也应该认为是utf8格式
sub isOnlyISO88591 {
    my $file = shift;
    my %chset;
    open(FILE, $file);
    while (<FILE>) {
        my $ch = CharsetDetector::detect($_);
        $chset{$ch} = 0;
    }
    close FILE;
    my @arr = %chset;
    
    if (scalar @arr == 2 && $arr[0] eq 'iso-8859-1') {
        return 1;
    }
    return 0;
}

sub isUTF8 {
    my $file = shift;
    # 如果没有找到utf8，就用iso-8859-1来判断
    if (!isFileChset("$file", 'utf8')) {
        return isOnlyISO88591($file);
    }
    return 1;
}

sub isGBK {
    my $file = shift;
    return isFileChset("$file", 'cp936');
}

