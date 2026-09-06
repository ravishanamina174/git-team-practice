
DROP DATABASE IF EXISTS calculator_system_db;
CREATE DATABASE calculator_system_db;
USE calculator_system_db;

-- ---------------------------------------------------------
-- Table: operations
-- Stores the valid operation types
-- ---------------------------------------------------------
CREATE TABLE operations (
    operation_id INT PRIMARY KEY AUTO_INCREMENT,
    operation_code VARCHAR(10) NOT NULL UNIQUE,
    operation_name VARCHAR(50) NOT NULL,
    description VARCHAR(255)
);

INSERT INTO operations (operation_code, operation_name, description) VALUES
('add', 'Addition', 'Adds two numbers together'),
('sub', 'Subtraction', 'Subtracts second number from first'),
('mul', 'Multiplication', 'Multiplies two numbers'),
('div', 'Division', 'Divides first number by second'),
('pow', 'Power', 'Raises first number to power of second'),
('sqrt', 'Square Root', 'Calculates square root of first number');

-- ---------------------------------------------------------
-- Table: users
-- Stores dummy user info associated with calculations
-- ---------------------------------------------------------
CREATE TABLE users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) NOT NULL UNIQUE,
    full_name VARCHAR(100),
    email VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO users (username, full_name, email) VALUES
('sample_user1', 'Sample One', 'sample1@example.com'),
('sample_user2', 'Sample Two', 'sample2@example.com'),
('sample_user3', 'Sample Three', 'sample3@example.com'),
('sample_user4', 'Sample Four', 'sample4@example.com'),
('sample_user5', 'Sample Five', 'sample5@example.com');

-- ---------------------------------------------------------
-- Table: calculation_records
-- Main table mirroring the C struct CalculationRecord
-- ---------------------------------------------------------
CREATE TABLE calculation_records (
    id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT,
    name VARCHAR(50) NOT NULL,
    num1 DOUBLE NOT NULL,
    num2 DOUBLE NOT NULL,
    operation VARCHAR(10) NOT NULL,
    result DOUBLE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (operation) REFERENCES operations(operation_code)
);

-- ---------------------------------------------------------
-- Dummy Insert Data (matches generateSampleData in C code)
-- ---------------------------------------------------------
INSERT INTO calculation_records (user_id, name, num1, num2, operation, result) VALUES
(1, 'Sample1', 10, 5, 'add', 15),
(2, 'Sample2', 20, 8, 'sub', 12),
(3, 'Sample3', 6, 7, 'mul', 42),
(4, 'Sample4', 100, 4, 'div', 25),
(5, 'Sample5', 2, 10, 'pow', 1024),
(1, 'Sample6', 81, 0, 'sqrt', 9),
(2, 'Sample7', 15, 15, 'add', 30),
(3, 'Sample8', 50, 25, 'sub', 25),
(4, 'Sample9', 9, 9, 'mul', 81),
(5, 'Sample10', 200, 10, 'div', 20);

-- ---------------------------------------------------------
-- Table: calculation_log
-- Tracks changes/updates to calculation_records (dummy audit log)
-- ---------------------------------------------------------
CREATE TABLE calculation_log (
    log_id INT PRIMARY KEY AUTO_INCREMENT,
    record_id INT,
    action VARCHAR(20),
    old_num1 DOUBLE,
    old_num2 DOUBLE,
    new_num1 DOUBLE,
    new_num2 DOUBLE,
    action_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (record_id) REFERENCES calculation_records(id)
);

INSERT INTO calculation_log (record_id, action, old_num1, old_num2, new_num1, new_num2) VALUES
(2, 'UPDATE', 20, 8, 100, 50),
(5, 'DELETE', 2, 10, NULL, NULL);

-- ---------------------------------------------------------
-- Views
-- ---------------------------------------------------------

-- View: summary of results per operation
CREATE VIEW view_operation_summary AS
SELECT
    operation,
    COUNT(*) AS total_records,
    AVG(result) AS avg_result,
    MAX(result) AS max_result,
    MIN(result) AS min_result
FROM calculation_records
GROUP BY operation;

-- View: records with user details
CREATE VIEW view_records_with_users AS
SELECT
    cr.id,
    u.username,
    u.full_name,
    cr.name,
    cr.num1,
    cr.num2,
    cr.operation,
    cr.result,
    cr.created_at
FROM calculation_records cr
JOIN users u ON cr.user_id = u.user_id;

-- ---------------------------------------------------------
-- Dummy Queries (for demonstration only, not executed here)
-- ---------------------------------------------------------

-- Select all records
SELECT * FROM calculation_records;

-- Select all records ordered by result ascending
SELECT * FROM calculation_records ORDER BY result ASC;

-- Select records for a specific operation
SELECT * FROM calculation_records WHERE operation = 'add';

-- Update a record (matches updateRecord in C code)
UPDATE calculation_records
SET num1 = 100, num2 = 50, result = 150
WHERE id = 2;

-- Delete a record (matches deleteRecord in C code)
DELETE FROM calculation_records WHERE id = 5;

-- Get average result (matches calculateAverage in C code)
SELECT AVG(result) AS average_result FROM calculation_records;

-- Get max result (matches findMaxResult in C code)
SELECT MAX(result) AS max_result FROM calculation_records;

-- Get min result (matches findMinResult in C code)
SELECT MIN(result) AS min_result FROM calculation_records;

-- Count records by operation (matches countByOperation in C code)
SELECT operation, COUNT(*) AS operation_count
FROM calculation_records
GROUP BY operation;

-- View statistics summary
SELECT * FROM view_operation_summary;

-- View records joined with user info
SELECT * FROM view_records_with_users;

-- ---------------------------------------------------------
-- Stored Procedure: Add a new calculation record
-- (dummy procedure, mirrors addRecord() in C code)
-- ---------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE sp_add_calculation_record (
    IN p_user_id INT,
    IN p_name VARCHAR(50),
    IN p_num1 DOUBLE,
    IN p_num2 DOUBLE,
    IN p_operation VARCHAR(10)
)
BEGIN
    DECLARE v_result DOUBLE;

    CASE p_operation
        WHEN 'add' THEN SET v_result = p_num1 + p_num2;
        WHEN 'sub' THEN SET v_result = p_num1 - p_num2;
        WHEN 'mul' THEN SET v_result = p_num1 * p_num2;
        WHEN 'div' THEN SET v_result = p_num1 / p_num2;
        WHEN 'pow' THEN SET v_result = POWER(p_num1, p_num2);
        WHEN 'sqrt' THEN SET v_result = SQRT(p_num1);
        ELSE SET v_result = 0;
    END CASE;

    INSERT INTO calculation_records (user_id, name, num1, num2, operation, result)
    VALUES (p_user_id, p_name, p_num1, p_num2, p_operation, v_result);
END$$

DELIMITER ;

-- Example call to the stored procedure (dummy usage)
-- CALL sp_add_calculation_record(1, 'ProcSample', 12, 4, 'div');

-- ---------------------------------------------------------
-- Trigger: Log updates to calculation_records
-- (dummy trigger, mirrors calculation_log usage)
-- ---------------------------------------------------------
DELIMITER $$

CREATE TRIGGER trg_after_update_calculation
AFTER UPDATE ON calculation_records
FOR EACH ROW
BEGIN
    INSERT INTO calculation_log (record_id, action, old_num1, old_num2, new_num1, new_num2)
    VALUES (OLD.id, 'UPDATE', OLD.num1, OLD.num2, NEW.num1, NEW.num2);
END$$

DELIMITER ;

-- ---------------------------------------------------------
-- End of File
-- ---------------------------------------------------------