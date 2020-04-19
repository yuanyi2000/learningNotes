# Git常用命令 #

## 创建版本库 ##
1. 配置信息
	`$git config --global user.name "YourName"`
	`$git config --global user.email "YourEmail"`

2. 创建空目录

```shell
	$mkdir learnGit/
	$cd learnGit/
	$pwd
	~/users/someBody/learnGit/
	$ git init
```

3. 创建和提交文件
```
	$git add readme.md
	$git commit -m "本次提交说明"

	$git status
	查看当前工作区状态

	$git diff readme.md
	比较文件的不同

	$git checkout --readme.md
	撤销对文件的修改
```

## 版本回退 ##
```
	$git log
	查看历史版本
	$git log --pretty=oneline
	使历史提交用一列打出
	$git reset --hard HEAD^
	回退到上一个版本 HEAD^^ HEAD~100
```

## 分支管理 ##

1. 创建分支
	`$git branch dev`

2. 切换分支
	`$git checkout dev`
	`$git switch dev`

3. 创建并切换到新分支
	`$git ckeckout -b dev`
	`$git switch -c dec`
4. 合并分支
`$git merge dev`

5. 删除分支
`$git branch -d dev`
`$git branch -D dev 强制删除`

6. 禁用Fast Forward并合并
`$git merge -no-ff -m "说明" dev`

## Bug分支 ##
1. 储存当前工作进度
`$git stash`

2. 处理bug
```
	$git switch master
	$git checkout -b issue-001
	修复bug
	$git add issue-001
	$git commit -m "修复了bug001" 
	$git switch master
	$git merge --no-ff -m "001" issue-001
	$git switch dev
```
3. 继续工作
```
	$git stach list
	$git stash pop / $git stash apply XXX
```

## 多人协作 ##
1. 查看远程库信息
`$git remote (-v)`
2. 推送分支
`$git push`
3. 当出现了冲突，抓取最新提交
`$git pull`
4. 查看直线提交历史
`$git rebase`
`$git log`

## 标签管理 ##
1. 切换到需要标签的分支然后设置标签
`$git branch`
`$git checkout master`
`$git tag <name>`

2. 查看所有标签
`$git tag`

3. 对历史提交打印标签
`$git log --pretty=oneline --abbrer-commit`
`$git tag <name> <commit id>`

4. 查看标签信息
`$git show <tagname>`

5. 创建带说名的标签
`$git tag -a <name> -m "说明" <commit id>`

6. 删除标签
`$git tag -d <tag name>`

7. 推送标签
`$git push origin <tagname>`
推送全部
`$git push origin tags`

8. 删除远程标签
	1. 删除本地
	`$git tag -d <name>`
	2. 推送删除消息到远程
	`$git push origin :refs/tags/<name>`

## 使用GitHub ##
1. 创建ssh key
`$ssh-keygen -t rsa -C "email@xx.com"`
`$~/.ssh/id_rsa id_rsa.pub`

2. 登录github，将id_rsa.pub的内容粘贴到SSH key
3. 添加远程库

## 自定义Git ##
1. 让git显示颜色
`$git config --global color.ui true`

2. 用.gitignore忽略特殊文件
	+ 强制添加
	`$git add -f xxx`
	+ 检查ignore
	`$git check-ignore`

3. 配置别名
使用AAA替代BBB
`$git config --global alias AAA BBB`


