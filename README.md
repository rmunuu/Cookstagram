# Cookstagram: Social Media Style Recipe Archive System  

<img src="image.png" alt="Cookstagram logo" width="300"/>  

## 1. Features of Cookstagram  
- **Feed System** (like Instagram) to share recipes and encourage interactions.  
- **Influencer Chefs** who interact with the public to help reduce the *cooking knowledge gap*.  
- **Query Search System** for efficient recipe searches.  
- **Diet Recommendations** based on calorie analysis.  

---

## 2. Command System  

<img src="image-1.png" alt="command system" width="500"/>  

Navigate through the sections using the commands shown above.  

---

## 3. User Manual  

### 1. Sign In/Sign Up  

```  
sign in [1]  
sign up [2]  
exit    [-1]  
```  

This is the initial page displayed after launching the program. Type the number corresponding to your desired action (e.g., 1 to sign in) and press Enter.  

### 2. Menu (after signing in)  

```  
<HOME>  
FEED      [1]  
SEARCH    [2]  
MY PAGE   [3]  
RECOMMEND [4]  
EXIT      [5]  
select mode:  
```  

After signing in, this menu will appear. Similarly, type the corresponding number and press Enter to navigate.  

### 3. FEED  

```  
Title: Woojin's Special Tiramisu (Easy and Delicious!!!)  
Username: Woojin Kim  
----------------------------------------------------------  
Required Ingredients  

Mascarpone Cheese                   100.00g  
Whipped Cream                       100.00g  
Castella                            200.00g  
Cocoa Powder                         20.00g  
Sugar                                30.00g  
Espresso                            100.00mL  
----------------------------------------------------------  
Cooking Instructions  

1. Add 30g of sugar to the whipped cream.  
2. Whip the mixture until it becomes creamy.  
...  
```  

The recipes will appear in the FEED section like the example above. Only 30 lines of a recipe are displayed at once. For longer recipes, use navigation commands (`u` and `d`) to move between pages.  

Available commands in the FEED section:  
- `a`: Show another recipe.  
- `d`: Move to the next page.  
- `u`: Move to the previous page.  
- `g`: Like the recipe.  
- `c`: Write a comment.  
- `s`: Save the recipe to your library with a memo.  

These commands are always available on the recipe display page.  

### 4. SEARCH  

```  
Search queries should be written in the following format:  
text=[]/tag=[]&[]/code=[]/ingredient=[]&[]/option=[like/tag/ingredient]  

--- Example ---  
text=hello/tag=egg/code=/ingredient=beef&milk/option=like  

Enter `q` to quit.  

Enter your search query:  
```  

This is the start screen of the SEARCH section. Enter a properly formatted query to find recipes that match your criteria.  

```  
> 100002 Woojin's Special Tiramisu (Easy and Delicious!!!)  
100010 Strawberry Ice Cream for Babies  
100011 Spicy Chicken Noodle Soup (Easiest in the World)  
102025 Potato Chips (Better than Lay's)
...  
```  

The search results matching your query will be displayed.  

Available commands:  
- `d`: Move the cursor down.  
- `u`: Move the cursor up.  
- `s`: Select the recipe the cursor is on and display its details.  

After selecting a recipe, you can view it (similar to how recipes are displayed in the FEED). Use `q` to return to the search results list.  

### 5. MY PAGE  

```  
<MY PAGE>  
MEMOS         [1]  
MODIFICATION  [2]  
HOME          [3]  
Select mode:  
```  

This screen appears when you access MY PAGE.  

In **MEMOS**, you can view saved recipes along with the memos you left.  

```  
<MEMOS>  
100011 - Next Sunday Dinner  
102025 - Kinsey will like it  
...  
```  

Use the recipe code to find the recipe in the SEARCH section.  

In **MODIFICATION**, you can change your password. Enter your current password first, then set the new one.  

### 6. RECOMMEND  

```  
Input your options. Type "help" if you need help. 
```  

This is the start screen of the RECOMMEND section. Enter "help" to see instructions and available commands.  

Available commands:  
- `cal`: View recipes sorted by calorie content.  
- `protein`: View recipes sorted by protein content.  
- `fat`: View recipes sorted by fat content.  
- `lcar_hfat`: View recipes that are low-carb and high-fat.  
- `today_menu`: Get today’s recommendation.  
- `:q`: Return to HOME.  

### 7. Other Resources

[**Interface Definition (Korean)**](https://github.com/rmunuu/Cookstagram/tree/master/Interface_definition.docx)  
