using System;
using System.Collections.Generic;

namespace CSharpLearningApp
{
    // 1. OBJECT-ORIENTED PROGRAMMING (OOP): Defining a Class
    class Product
    {
        // Properties (Auto-implemented)
        public string Name { get; set; }
        public double Price { get; set; }

        // Constructor to initialize the object
        public Product(string name, double price)
        {
            Name = name;
            Price = price;
        }

        // Method belonging to the class
        public void DisplayDetails()
        {
            Console.WriteLine($"- {Name}: ${Price:F2}");
        }
    }

    class Program
    {
        // The Main method is the exact entry point of every C# console application
        static void Main(string[] sender)
        {
            // --- HEADER ---
            Console.ForegroundColor = ConsoleColor.Cyan;
            Console.WriteLine("========================================");
            Console.WriteLine("    WELCOME TO THE C# LEARNING APP      ");
            Console.WriteLine("========================================\n");
            Console.ResetColor();

            // 2. VARIABLES & DATA TYPES
            string studentName = "Alex"; 
            int targetLessons = 5;       
            bool isMotivated = true;     

            Console.WriteLine($"Hello, {studentName}! Welcome to your C# crash course.");
            Console.WriteLine($"Your goal today is to master {targetLessons} foundational concepts.");
            Console.WriteLine($"Are you ready to code? {isMotivated}\n");

            // 3. CONDITIONAL STATEMENTS (Control Flow)
            Console.WriteLine("--- Checking Status ---");
            if (targetLessons >= 5 && isMotivated)
            {
                Console.WriteLine("Status: You are on track for accelerated learning!");
            }
            else if (targetLessons < 5 && isMotivated)
            {
                Console.WriteLine("Status: A relaxed, steady approach to coding.");
            }
            else
            {
                Console.WriteLine("Status: Remember, consistency is key!");
            }
            Console.WriteLine();

            // 4. LOOPS (Counting and Repeating)
            Console.WriteLine("--- Counting Finished Modules (For Loop) ---");
            for (int i = 1; i <= 3; i++)
            {
                Console.WriteLine($"Successfully completed milestone #{i}");
            }
            Console.WriteLine();

            // 5. DATA STRUCTURES: Working with Collections (Lists)
            Console.WriteLine("--- Fetching Inventory Items (Lists & Foreach) ---");
            List<Product> storeInventory = new List<Product>();

            // Creating objects using our custom class and adding them to the list
            storeInventory.Add(new Product("C# Programming Guide", 29.99));
            storeInventory.Add(new Product("Mechanical Keyboard", 89.50));
            storeInventory.Add(new Product("Ergonomic Mouse", 45.00));

            // Iterating through the list using a foreach loop
            foreach (Product item in storeInventory)
            {
                item.DisplayDetails();
            }
            Console.WriteLine();

            // 6. EXCEPTION HANDLING (Preventing Crashes)
            Console.WriteLine("--- Math Division Simulation (Try-Catch) ---");
            try
            {
                Console.Write("Enter a number to divide 100 by: ");
                string input = Console.ReadLine();
                
                // Converting string input safely to an integer
                int divisor = int.Parse(input); 
                int result = 100 / divisor;
                
                Console.WriteLine($"Success! 100 divided by {divisor} is: {result}");
            }
            catch (DivideByZeroException)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("Error: You cannot divide a number by zero!");
                Console.ResetColor();
            }
            catch (FormatException)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("Error: Invalid entry. Please enter whole numbers only.");
                Console.ResetColor();
            }
            finally
            {
                Console.WriteLine("Division test execution complete.");
            }
            Console.WriteLine();

            // 7. CALLING A CUSTOM STATIC METHOD
            double finalScore = CalculateCompletionRate(3.0, (double)targetLessons);
            Console.WriteLine($"Your current course completion rate is: {finalScore}%\n");

            // --- FOOTER ---
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("========================================");
            Console.WriteLine("  Press any key to close this program   ");
            Console.WriteLine("========================================");
            Console.ResetColor();
            Console.ReadKey(); 
        }

        // 8. METHODS / FUNCTIONS: Simple reusable code block
        static double CalculateCompletionRate(double completed, double total)
        {
            if (total == 0) return 0;
            return (completed / total) * 100;
        }
    }
}
