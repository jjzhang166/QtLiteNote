use 5.010;
use warnings;
use strict;

sub main;
sub HaveQObject;
sub ui;
sub moc;

main;

sub main
{
    say "\n**********moc**********";
    moc;
    say "\n**********ui**********";
    ui;
    system("rcc -name temp -no-compress ras.qrc -o ras.cpp");
}

sub ui
{
    while (<*.ui>) {
        my @fs = split /\./, $_;
        my $h_file = "ui_$fs[0].h";
        system("uic $_ -o $h_file");

        say "create $h_file";
    }
}

sub moc
{
    while (<*.h>) {
        my $f = $_;
        if (HaveQObject($_)) {
            my @fs = split /\./, $f;
            my $c_file = "moc_$fs[0].cpp";
            system("moc.exe $f -o $c_file");

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

