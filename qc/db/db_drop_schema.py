from __future__ import print_function
from __future__ import absolute_import
# Copyright (c) 2015-2016, Danish Geodata Agency <gst@gst.dk>
# Copyright (c) 2016, Danish Agency for Data Supply and Efficiency <sdfe@sdfe.dk>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
from builtins import input
from builtins import str
import os,sys
import argparse
import psycopg2
try:
	from  .pg_connection import PG_CONNECTION
except Exception as e:
	print("Failed to import pg_connection.py - you need to specify the keyword PG_CONNECTION!")
	print(str(e))
	raise e

parser=argparse.ArgumentParser(description="Drop a Postgis schema.")
parser.add_argument("schema",help="The name of the schema to create.")




def main(args):
	pargs=parser.parse_args(args[1:])
	PSYCOPGCON = PG_CONNECTION.replace("PG:","").strip()
	conn = psycopg2.connect(PSYCOPGCON)
	cur=conn.cursor()
	s=input("Are you sure you want to drop the schema "+pargs.schema+" ? (Yes/no): ")
	if s.strip().lower().startswith("yes"):
		MyCommand = "DROP SCHEMA IF EXISTS "+pargs.schema+" CASCADE"
		cur.execute(MyCommand)
		conn.commit()
	else:
		print("OK.. quitting.")
	cur.close()
	conn.close()

if __name__=="__main__":
	main(sys.argv)
