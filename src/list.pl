use 5.010;
use warnings;
use strict;
use CharsetDetector;
use CharsetDetector qw(detect detect1);

sub main;
sub isUTF8;
sub isGBK;

main;

sub main {
#    printf "%-20s utf8", "sdf";
#    return;

    opendir(DIR, ".");
    my @files = readdir(DIR);

    my $maxLen = 0;
    foreach (@files) {
        my $len = length($_);
        if ($len > $maxLen) {
            $maxLen = $len;
        }
    }
    $maxLen = $maxLen + 8;

    foreach (@files) {
        my $f = $_;
        if (-T $_) {
            if (isUTF8($f)) {
                printf "%-$maxLen"."s UTF8", $f;
            } elsif (isGBK($f)) {
                printf "%-$maxLen"."s GBK", $f;
            } else {
                printf "%-$maxLen"."s Unkown", $f;
            }
            print "\n";
        }
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

sub isUTF8 {
    my $file = shift;
    return isFileChset("$file", 'utf8');
}

sub isGBK {
    my $file = shift;
    return isFileChset("$file", 'cp936');
}
