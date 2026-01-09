# ===============================
# CONFIGURATION
# ===============================
# Change this URL to your new GitHub repo
$NEW_REPO_URL = "https://github.com/IhsanKhann/LoneWolfGame.git"

# ===============================
# SCRIPT START
# ===============================
Write-Host "Cleaning old Git history..."
# Delete old .git folder
Remove-Item -Recurse -Force .git

Write-Host "Initializing new Git repository..."
git init

Write-Host "Adding all files..."
git add .

Write-Host "Creating initial commit..."
git commit -m "Initial commit with cleaned history"

Write-Host "Adding remote repository..."
git remote add origin $NEW_REPO_URL

Write-Host "Renaming branch to main..."
git branch -M main

Write-Host "Force pushing to remote..."
git push -u origin main --force

Write-Host "✅ Repo cleaned and pushed successfully!"