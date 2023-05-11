#!/usr/bin/expect

# 设置传输速度为 xx/1024 MB每秒
set speed 204800

set timeout 30

if { $argc < 1 } {
	puts "worng! argv0 is scp file."
	exit 1
}


set self_file [lindex $argv 0]
set obj_file [lindex $argv 1]
set password "root"

spawn scp -l $speed  $self_file root@172.16.13.10:/xlian/

expect {
	"(yes/no" {
		send "yes\r" 
		expect "*assword:" {
			send "$password\r"
		}
	}
	
	"password:" { send "$password\r" }
}


interact 

