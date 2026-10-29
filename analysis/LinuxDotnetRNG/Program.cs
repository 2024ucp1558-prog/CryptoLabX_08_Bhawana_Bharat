using System;
using System.IO;
using System.Security.Cryptography;

byte[] data = RandomNumberGenerator.GetBytes(1048576);
File.WriteAllBytes("../dotnet_rng.bin", data);
Console.WriteLine(new FileInfo("../dotnet_rng.bin").Length);
﻿// See https://aka.ms/new-console-template for more information
Console.WriteLine("Hello, World!");
