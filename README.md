# SALES MANAGER
#### Video Demo: https://youtu.be/IJDBW7gLrR4
#### Description:
This project is meant to represent a basic selling and spending manager for local stores, built in a CLI interface in C. It includes features such as product registration, sales engine (daily sale registering), sales visualizer (visualizing sales for one or mutiple days) and and expendings manager, with listings dated by the user. All piece of this software contains input validation to avoid bugs and is EXCLUSIVELY COMPATIBLE WITH MICROSOFT WINDOWS.

Observation: the project was initially made in Brazilian Portuguese.

1) The Main Menu
The main menu is simply what you see at launch and allow access to the program's functions, doing nothing by itself. The other files contain the de facto purposes of the software.
2) The Product Registerer
This feature is meant to register what products the user is going to sell at their store. It has 4 different subfunctions:
a. Registering: here, the user specifies the name of the product and the unit sale price. The program returns the product ID (used in other functions) and registers the created product into a file with ID, name and price.
b. Listing: here, the user can see all of the registered products (upon parsing the product file) and all of their items: ID, name and price.
c. Editing: here, the user can edit the name or price of the chosen product. Since it there's no too much of a good reason to do it, editing IDs is not allowed (you must reregister the product to change ID and delete the old one). The same way both traits can be edited, the user may opt to not change one of them.
d. Deleting: the user specifies a product ID and, upon confirmation, deletes the product listing from the file.
WARNING: deleted products will NOT appear in previous sales that contained them. Use this feature wisely.
3) The Sales Engine
This feature is the seller's daily driver: during the day, sales will be registered here. Upon launching the feature, the program creates a file named as the current date. It then starts the following loop

a. Asking the product ID of the sale (the one registered earlier in the Product Registerer)

b. Asking the amount of products sold

c. Asking if the sale will include another, different product (limited 30 different products/sale). If yes, loops back to a.

d. Asking the payment method of the sale. Notice the screen contains Pix as a payment method, due to the program being originally meant to be used in Brazil.

e. The sale is finalized, and the sold products are registered upon that file named after the current date. The prices are parsed upon the product's listing registered beforehand in the Registerer.

The program continues to read sales until the user says they don't want to sell anymore in that day.

4) The Sales Visualizer

This feature is the most complex of all of them, containing two forms of visualizing

a. Single day

This mode reads the date inserted by the user, and searches the file that matches that date. After this, it reads the entire file and outputs the  1. All of the different products sold on the day, and amount

2. The gross income of the day, separated by payment method

3. The net income of the day, separated by payment method. This is calculated upon discounting the card machine fees, hardcoded following a real machine's ones, for credit, debit and Pix.

4. The gross and net income of the day overall.

b. Multiple days

This mode asks for the start and ending dates of the desired sales period. Upon validation, the program checks if a file exists for the initial date, and if yes, it begins counting all its sales' income per payment method. After the check of the file ends, it increases the day by one and repeats the process, adding values to a total period count, until the final date informed by the user. It then displays the gross and net incomes of the period, separated by payment method, and the overall count. Due to the insane complexity of how this is done in C, it was opted to NOT include the names and amounts of sold products in the period.

5) The Expendings Registerer

Alongside the profits, registering and tracking the investments and spendings is as important to maintain a company running. This feature is meant to cover this issue.
It contains 4 features.

a. Creating expense tables: it's asked for the user the initial and final date covered by the spending list. It then begins to ask for each spending's name and amount. After each spending is added, the program asks the user if they want to continue listing spendings on that table, with a maximum of 30 different entries per table. After the user says no (or the limit is reached), it creates an ID for that table and a file for it inside a dedicated folder. It also calculates the total amount of the table and outputs the table's metadata to the user: ID, initial and final date covered, and the total amount. These metadata are also added to a file that lists the tables.

b. Viewing available expense tables: remember I told you there's a file that registers the metadata of the sales? This function basically reads it and outputs all of its information into the user's screen.

c. Deleting an expense table: this function asks for a table's ID, and, upon confirmation, deletes the file containing that table and its listing from the available tables file.

d. Viewing a expense table (or detailed expenses, as in the CLI): it asks the user for a table's ID, and outputs it's contents: the expenses' names and their amounts. It does not show the total amount, which can be seen in function b.

Overall, this project was a really challeging, final worthy program that despite simple, is insanely hard to do it in such a low level language as C. I hope this is able to showcase my programming skills and maybe even be useful for a local seller someday.

This is Guilherme Miyasaki from Brazil, and his final project, for CS50.