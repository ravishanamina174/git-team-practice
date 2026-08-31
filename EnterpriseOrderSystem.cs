using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace EnterpriseOrderSystem
{
    #region Core Models & Enums

    public enum OrderStatus { Pending, Validated, Paid, Shipped, Failed }
    public enum PaymentMethod { CreditCard, PayPal, Crypto }

    public class OrderItem
    {
        public string ProductId { get; set; } = string.Empty;
        public string ProductName { get; set; } = string.Empty;
        public decimal UnitPrice { get; set; }
        public int Quantity { get; set; }
        public decimal TotalPrice => UnitPrice * Quantity;
    }

    public class Order
    {
        public string OrderId { get; } = Guid.NewGuid().ToString("N");
        public string CustomerId { get; set; } = string.Empty;
        public List<OrderItem> Items { get; set; } = new();
        public PaymentMethod PaymentMethod { get; set; }
        public OrderStatus Status { get; set; } = OrderStatus.Pending;
        public decimal TotalAmount => Items.Sum(item => item.TotalPrice);
        public string FailureReason { get; set; } = string.Empty;
    }

    #endregion

    #region Custom Exceptions

    public class OrderProcessingException : Exception
    {
        public string OrderId { get; }
        public OrderProcessingException(string message, string orderId) : base(message) => OrderId = orderId;
    }

    public class InventoryException : OrderProcessingException
    {
        public InventoryException(string message, string orderId) : base(message, orderId) { }
    }

    public class PaymentException : OrderProcessingException
    {
        public PaymentException(string message, string orderId) : base(message, orderId) { }
    }

    #endregion

    #region Services & Strategies

    // Strategy Pattern for Payments
    public interface IPaymentStrategy
    {
        PaymentMethod Method { get; }
        Task<bool> ProcessPaymentAsync(string orderId, decimal amount);
    }

    public class CreditCardPayment : IPaymentStrategy
    {
        public PaymentMethod Method => PaymentMethod.CreditCard;
        public async Task<bool> ProcessPaymentAsync(string orderId, decimal amount)
        {
            await Task.Delay(150); // Simulate bank gateway latency
            return amount < 10000; // Decline transactions over $10,000 for fraud safety
        }
    }

    public class PayPalPayment : IPaymentStrategy
    {
        public PaymentMethod Method => PaymentMethod.PayPal;
        public async Task<bool> ProcessPaymentAsync(string orderId, decimal amount)
        {
            await Task.Delay(100);
            return true;
        }
    }

    // Thread-Safe Inventory Manager
    public class InventoryManager
    {
        private readonly ConcurrentDictionary<string, int> _stock = new();

        public void Restock(string productId, int quantity) => 
            _stock.AddOrUpdate(productId, quantity, (_, current) => current + quantity);

        public async Task<bool> ReserveStockAsync(Order order)
        {
            await Task.Delay(50); // Simulate database operation
            
            // Atomic inventory check and reduction
            lock (_stock)
            {
                foreach (var item in order.Items)
                {
                    if (!_stock.TryGetValue(item.ProductId, out int available) || available < item.Quantity)
                    {
                        return false;
                    }
                }

                foreach (var item in order.Items)
                {
                    _stock.TryUpdate(item.ProductId, _stock[item.ProductId] - item.Quantity, _stock[item.ProductId]);
                }
            }
            return true;
        }
    }

    #endregion

    #region Pipeline Engine

    public class OrderPipelineEngine
    {
        private readonly InventoryManager _inventory;
        private readonly Dictionary<PaymentMethod, IPaymentStrategy> _paymentStrategies;
        private readonly ConcurrentBag<Order> _completedOrders = new();

        public IEnumerable<Order> CompletedOrders => _completedOrders;

        public OrderPipelineEngine(InventoryManager inventory, IEnumerable<IPaymentStrategy> paymentStrategies)
        {
            _inventory = inventory;
            _paymentStrategies = paymentStrategies.ToDictionary(s => s.Method);
        }

        public async Task ProcessOrderAsync(Order order)
        {
            try
            {
                Console.WriteLine($"[START] Processing Order {order.OrderId} for Customer {order.CustomerId}");

                // Step 1: Validation
                if (!order.Items.Any()) 
                    throw new OrderProcessingException("Order contains no items.", order.OrderId);

                order.Status = OrderStatus.Validated;

                // Step 2: Inventory Reservation
                bool stockReserved = await _inventory.ReserveStockAsync(order);
                if (!stockReserved) 
                    throw new InventoryException("Insufficient stock available for requested items.", order.OrderId);

                // Step 3: Payment Gateway
                if (!_paymentStrategies.TryGetValue(order.PaymentMethod, out var strategy))
                    throw new PaymentException($"Payment method {order.PaymentMethod} not supported.", order.OrderId);

                bool paymentSuccess = await strategy.ProcessPaymentAsync(order.OrderId, order.TotalAmount);
                if (!paymentSuccess)
                    throw new PaymentException("Payment authorization declined by provider.", order.OrderId);

                order.Status = OrderStatus.Paid;

                // Step 4: Fulfillment/Shipping Handoff
                await Task.Delay(80); // Simulate warehouse logistics notification
                order.Status = OrderStatus.Shipped;

                _completedOrders.Add(order);
                Console.WriteLine($"[SUCCESS] Order {order.OrderId} is now SHIPPED.");
            }
            catch (OrderProcessingException ex)
            {
                order.Status = OrderStatus.Failed;
                order.FailureReason = ex.Message;
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"[FAILURE] Order {ex.OrderId} failed during stage {ex.GetType().Name}: {ex.Message}");
                Console.ResetColor();
            }
        }
    }

    #endregion

    #region Execution Entry Point

    public class Program
    {
        public static async Task Main()
        {
            Console.WriteLine("=== Initializing Enterprise Order Processing Pipeline ===");

            // 1. Setup Infrastructure
            var inventory = new InventoryManager();
            inventory.Restock("PROD_001", 10);
            inventory.Restock("PROD_002", 2);

            var paymentStrategies = new List<IPaymentStrategy>
            {
                new CreditCardPayment(),
                new PayPalPayment()
            };

            var engine = new OrderPipelineEngine(inventory, paymentStrategies);

            // 2. Generate Mock Batch Orders
            var batchOrders = new List<Order>
            {
                new() { 
                    CustomerId = "CUST_A", PaymentMethod = PaymentMethod.PayPal,
                    Items = new() { new() { ProductId = "PROD_001", ProductName = "Laptop", UnitPrice = 1200m, Quantity = 1 } }
                },
                new() { 
                    CustomerId = "CUST_B", PaymentMethod = PaymentMethod.CreditCard, // Will fail: Overdraft limit safety
                    Items = new() { new() { ProductId = "PROD_001", ProductName = "High-End Server", UnitPrice = 15000m, Quantity = 1 } }
                },
                new() { 
                    CustomerId = "CUST_C", PaymentMethod = PaymentMethod.CreditCard, // Will fail: Out of stock
                    Items = new() { new() { ProductId = "PROD_002", ProductName = "Monitor", UnitPrice = 300m, Quantity = 5 } }
                }
            };

            // 3. Concurrent Engine Execution
            Console.WriteLine($"\nLaunching processing for {batchOrders.Count} batch orders concurrently...\n");
            
            IEnumerable<Task> processingTasks = batchOrders.Select(order => engine.ProcessOrderAsync(order));
            await Task.WhenAll(processingTasks);

            // 4. Reporting Analytics (LINQ Expressions)
            Console.WriteLine("\n=== Order Processing Metrics Report ===");
            Console.WriteLine($"Total processed successfully: {engine.CompletedOrders.Count()}");
            Console.WriteLine($"Total revenue handled: ${engine.CompletedOrders.Sum(o => o.TotalAmount):N2}");
        }
    }

    #endregion
}
