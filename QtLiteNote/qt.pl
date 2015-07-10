use 5.010;
use warnings;
use strict;

sub main;
sub HaveQObject;
sub ui;
sub moc;
sub ras;

main;

sub main
{
    say "\n**********moc**********";
    moc;

    say "\n**********ui**********";
    ui;
    
    say "\n**********ras**********";
    ras;
}

#将ui转化为.h文件
sub ui
{
    while (<*.ui>) {
        my @fs = split /\./, $_;
        my $h_file = "ui_$fs[0].h";
        system("uic $_ -o $h_file");

        say "create $h_file";
    }
}

#将有元信息的.h文件生成对应的.cpp文件
sub moc
{
    while (<*.h>) {
        my $f = $_;
        if (HaveQObject($_)) {
            my @fs = split /\./, $f;
            my $c_file = "moc_$fs[0].cpp";
            system("moc $f -o $c_file");

            say "create $c_file";
        }
    }
}

sub HaveQObject
{
    my $filename = $_[0];
    open FILE, $filename;
    while (<FILE>) {
        chomp;
        s/^\s+//;
        s/\s+$//;
        if ("Q_OBJECT" eq $_) {
            return 1;
        }
    }
    return 0;
}

#将资源生成对应.h文件
sub ras
{
    while (<*.qrc>) {
        my $s = substr($_, 0, rindex($_, "."));
        my $c = "ras_$s.cpp";
        system("rcc -no-compress $_ -o $c");
        say $c;
    }
}
