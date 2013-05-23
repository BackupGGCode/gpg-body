/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) 2013 Uriel Fanelli <uriel.fanelli@gmail.com>
 * 
 * gpg-body is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gpg-body is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <iterator>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <functional>

// Tutti i commenti sono in italiano
// spiacente per i barbari

int main( int argc , char *argv[])
{
// ci servono anche gli spazi
	
  std::cin >> std::noskipws;

// leggiamo usando un iteratore
// perche' io valgo	
	
  std::istream_iterator<char> it(std::cin);
  std::istream_iterator<char> end;
  std::string email(it, end);
  
// Inizializziamo il randomizzatore congruente  
  
  std::srand(std::time(NULL));


// dividiamo in due l'email 

  std::string sepa[] = {"\n\n","\x0D\x0A\x0D\x0A"} ;
  std::string headers;	
  size_t confine;
	
	for(int i = 0; i < 2; i++)
	{
	//troviamo il confine tra header e body
		confine = email.find(sepa[i],0);
		if (confine != std::string::npos)
		{
		// dividiamo header e body	
		headers =  email.substr(0, confine);
		email =  email.substr(confine + 1);	
		}	
	}
		

//definiamo il comando gpg
//criptiamo e restituiamo ASCII	
	
	std::string gpg_command = "/usr/bin/gpg -ea ";

// vediamo che ci sia almeno il destinatario
// es: gpg -ea  -r uriel.fanelli@gmail.com 
	
	if (argc < 2 )
	{
		std::cout << headers;
		std::cout << email;
		std::exit(1);
	}	
		
// costruiamo il comando completo
	
    for(int i = 1; i < argc; i++)
	{
		
		gpg_command.append(argv[i]);
		gpg_command.append(" ");
	 
	}
gpg_command.append("\n");
	

// nessun exploit sulla riga di comando
	
    if (gpg_command.length() > 1023)
	{
        //se la riga e' troppo lunga restituiamo l'input intatto ed usciamo		
		std::cout << headers;
		std::cout << email;
		std::exit(1);
	}		
	
	

//  Iniziamo con le stringhe che ci servono per il content encoding

     char pwd[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHILMNOPQRSTUVZ";
	 char smime_uuid[16];

	 for(int i=0; i<16; i++) 
	    {
	        smime_uuid[i] = pwd[std::rand() % 47];
	    }

// Prendiamo l'header gia' presente nell' header originale
// Poi dobbiamo attaccarlo alla prima linea del body.	


   std::string header_orig;

   std::string strarr[] = {"Content-Type: ", "Content-disposition: ", "Content-transfer-encoding: "};
   size_t nFPos;
   size_t secondNL;
   

	for(int i = 0; i < 3; ++i) 
	{

	nFPos =    headers.find(strarr[i]);
    secondNL = headers.find('\n', nFPos); 
	
	if (nFPos != std::string::npos)
	        {
			 header_orig = headers.substr(nFPos,secondNL);
			 headers.erase(nFPos,secondNL);		 
			}	
			
     }
	
	header_orig.append("\n\n");
	

//  Aggiungiamo l'header mime per la codifica PGP
//  Content-Type: multipart/encrypted; protocol="application/pgp-encrypted";boundary=" XYZ "

	std::string smime_pgp_header;
	smime_pgp_header.append("Content-Type: multipart/encrypted; protocol=\"application/pgp-encrypted\";boundary=\"");
	smime_pgp_header.append(smime_uuid);
	smime_pgp_header.append("\"\n");
    headers.append(smime_pgp_header);

	
// Scriviamo la body enclosure	

	std::cout << headers  << std::endl;
    std::cout << "--" << smime_uuid << std::endl;
	std::cout << "Content-Type: application/pgp-encrypted"  << std::endl;
	std::cout << "Content-Disposition: attachment\n\nVersion: 1\n" << std::endl;
	std::cout << "--" << smime_uuid << std::endl;
	std::cout << "Content-Type: application/octet-stream" << std::endl;
	std::cout << "Content-Disposition: inline; filename=\"msg.asc\"\n" << std::endl;	

// apriamo una pipe con il comando gpg desiderato
	
	FILE *stream = popen(gpg_command.c_str(), "w");

//qui attacchiamo l'header originale per il mime
//il comando in  popen scrivera' sullo standard output perche' lo fa gpg
	
	fputs(header_orig.c_str(),stream);

// e qui il body originale
	
	fputs(email.c_str(), stream);

// fatto, adesso chiudiamo il pipe	

	pclose(stream);
	
// chiudiamo il mime	
	
	std::cout << "\n--" << smime_uuid << std::endl << std::endl;

// e salutiamo 

	std::exit(0);
	
//arrivederci e grazie	
	
}