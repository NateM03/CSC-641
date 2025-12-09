@echo off
cd /d "%~dp0"
echo Adding all files...
git add -A
echo.
echo Committing changes...
git commit -m "Add Milestone 3 complete implementation and documentation"
echo.
echo Setting remote...
git remote set-url origin https://github.com/NateM03/CSC-641.git
echo.
echo Switching to main branch...
git branch -M main
echo.
echo Pushing to GitHub...
echo (You may be prompted for GitHub credentials)
git push -u origin main --force
echo.
echo Done! Check GitHub to verify your files are uploaded.
pause
