# Tools Repository CI/CD Pipeline

This directory contains GitHub Actions workflows that ensure the quality, security, and reliability of the `hf-espidf-project-tools` repository.

## 🚀 Overview

The CI pipeline automatically validates every change to the repository using **5 parallel quality checks** that run on every push and pull request. This ensures the tools maintain high standards and work reliably for all users.

## 📋 CI Pipeline Jobs

### 1. **Lint Check** 🔍
**What it does:**
- **Python Code**: Formats code with `black`, sorts imports with `isort`, checks style with `flake8`
- **Shell Scripts**: Validates syntax and best practices with `shellcheck`
- **YAML Files**: Checks syntax and formatting with `yamllint`
- **Markdown**: Validates documentation formatting with `markdownlint`

**Files checked:**
- `generate_matrix.py`, `get_app_info.py` (Python)
- `build_app.sh`, `config_loader.sh`, `flash_app.sh`, etc. (Shell scripts)
- `.github/workflows/*.yml` (YAML configuration)
- `README.md`, `docs/*.md` (Documentation)

**Why it matters:**
- Ensures consistent code style across all contributors
- Catches syntax errors before they reach users
- Makes code easier to read and maintain

### 2. **Security Scan** 🛡️
**What it does:**
- **Dependency Scanning**: Checks `requirements.txt` for known vulnerabilities using `pip-audit` and `safety`
- **Secret Detection**: Scans for accidentally committed API keys, passwords, and tokens using `gitleaks`
- **Code Analysis**: Runs `bandit` to find security issues in Python code
- **Static Analysis**: Uses GitHub's CodeQL to find security vulnerabilities

**Files scanned:**
- `requirements.txt` (Python dependencies)
- All Python files for security issues
- Entire repository history for secrets

**Why it matters:**
- Prevents vulnerable dependencies from being used
- Stops accidental secret leaks
- Maintains security standards for a widely-used tool

### 3. **Documentation Validation** 📚
**What it does:**
- **Structure Check**: Verifies all required documentation files exist
- **Content Validation**: Ensures documentation is complete and properly formatted
- **Link Verification**: Checks internal documentation links work correctly
- **Generation**: Creates documentation index and summaries

**Files validated:**
- `README.md` (main documentation)
- `docs/README_*.md` (detailed guides)
- All shell script documentation headers

**Why it matters:**
- Ensures users can understand and use the tools effectively
- Prevents broken documentation links
- Maintains comprehensive user guides

### 4. **Static Analysis** 🔬
**What it does:**
- **Code Quality**: Analyzes Python code complexity and maintainability
- **Best Practices**: Checks for common programming mistakes and anti-patterns
- **Documentation**: Validates docstrings and code comments
- **Complexity**: Measures cyclomatic complexity to identify overly complex code

**Tools used:**
- `pylint` - Python code analysis
- `pydocstyle` - Docstring validation
- `radon` - Complexity analysis
- `xenon` - Complexity monitoring

**Why it matters:**
- Identifies code that's hard to maintain or debug
- Ensures consistent documentation standards
- Helps prevent bugs before they occur

### 5. **Link Check** 🔗
**What it does:**
- **Internal Links**: Verifies all markdown links within the repository work
- **External Links**: Checks that external URLs are accessible
- **Cross-References**: Ensures documentation references are valid
- **GitHub Links**: Validates GitHub repository and issue links

**Files checked:**
- `README.md` and all `docs/*.md` files
- All markdown files for broken links

**Why it matters:**
- Prevents users from encountering broken links
- Maintains professional documentation quality
- Ensures external resources are still available

## 🎯 CI Summary & Reporting

After all jobs complete, the CI generates a comprehensive summary that includes:

- **Status Table**: Shows which jobs passed/failed
- **Overall Status**: Clear indication of pipeline success
- **Next Steps**: Guidance on what to do if checks fail
- **PR Comments**: Automatic comments on pull requests with results

## ⚙️ Configuration

### Workflow Triggers
- **Push**: Runs on every push to `main` or `develop` branches
- **Pull Request**: Validates all pull requests before merging
- **Manual**: Can be triggered manually from GitHub Actions tab

### Reusable Workflows
The CI uses workflows from the main `N3b3x/hf-espidf-ci-tools` repository:
- `lint.yml@main` - Code formatting and style checking
- `security.yml@main` - Security vulnerability scanning
- `docs.yml@main` - Documentation validation
- `static-analysis.yml@main` - Code quality analysis
- `link-check.yml@main` - Link validation

### Input Parameters
- `project_dir: "."` - Uses current directory as project root
- `auto_clone_tools: false` - No need to clone tools (we are the tools)
- `paths: "**/*.py,**/*.sh,**/*.yml,**/*.yaml,**/*.md"` - Specific file types to check

## 🚨 Understanding CI Results

### ✅ **All Checks Passed**
- Code is ready for merge/deployment
- All quality standards met
- No security issues found
- Documentation is complete and valid

### ❌ **Some Checks Failed**
- Review the specific job outputs
- Fix the identified issues
- Push changes to re-run the pipeline
- Check the "Next Steps" section in the summary

### 🔍 **Common Issues & Solutions**

#### Lint Failures
- **Python**: Run `black .` and `isort .` to auto-fix formatting
- **Shell**: Fix `shellcheck` warnings in shell scripts
- **Markdown**: Address `markdownlint` style issues

#### Security Failures
- **Dependencies**: Update vulnerable packages in `requirements.txt`
- **Secrets**: Remove any accidentally committed secrets
- **Code Issues**: Fix security issues identified by `bandit`

#### Documentation Failures
- **Missing Files**: Ensure all required documentation exists
- **Broken Links**: Fix or remove broken internal/external links
- **Format Issues**: Correct markdown formatting problems

## 🛠️ Local Development

### Running Checks Locally
You can run similar checks locally before pushing:

```bash
# Install tools
pip install black isort flake8 shellcheck

# Python formatting
black --check --diff .
isort --check-only --diff .

# Python linting
flake8 . --max-line-length=100 --extend-ignore=E203,W503

# Shell linting
find . -name "*.sh" -exec shellcheck {} \;

# Security check
pip install pip-audit safety
pip-audit --requirement requirements.txt
safety check --requirement requirements.txt
```

### Pre-commit Hooks (Optional)
While not configured locally, you can set up pre-commit hooks:

```bash
pip install pre-commit
# Create .pre-commit-config.yaml with appropriate hooks
pre-commit install
```

## 📊 CI Performance

- **Total Runtime**: ~5-10 minutes (all jobs run in parallel)
- **Resource Usage**: Optimized for efficiency
- **Caching**: Uses GitHub Actions caching for faster runs
- **Artifacts**: Generates reports and summaries for download

## 🔄 CI Pipeline Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                    🚀 Push/PR Trigger                           │
└─────────────────────┬───────────────────────────────────────────┘
                      │
        ┌─────────────┼─────────────┐
        │             │             │
        ▼             ▼             ▼
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ 🔍 Lint     │ │ 🛡️ Security │ │ 📚 Docs     │
│ Check       │ │ Scan        │ │ Validation  │
└─────────────┘ └─────────────┘ └─────────────┘
        │             │             │
        └─────────────┼─────────────┘
                      │
        ┌─────────────┼─────────────┐
        │             │             │
        ▼             ▼             ▼
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ 🔬 Static   │ │ 🔗 Link     │ │             │
│ Analysis    │ │ Check       │ │             │
└─────────────┘ └─────────────┘ └─────────────┘
        │             │             │
        └─────────────┼─────────────┘
                      │
                      ▼
            ┌─────────────────┐
            │ 📊 CI Summary   │
            │ & Reporting     │
            └─────────┬───────┘
                      │
            ┌─────────┼─────────┐
            │         │         │
            ▼         ▼         ▼
       ┌─────────────┐ ┌─────────────┐
       │ 💬 PR       │ │ 📦 Artifact │
       │ Comment     │ │ Upload      │
       └─────────────┘ └─────────────┘

All jobs run in PARALLEL for maximum speed! ⚡
```

## 📈 CI Job Details

| Job | Runtime | Tools | Purpose |
|-----|---------|-------|---------|
| **Lint** | ~2-3 min | black, isort, flake8, shellcheck, yamllint, markdownlint | Code quality & style |
| **Security** | ~3-5 min | pip-audit, safety, bandit, gitleaks, CodeQL | Vulnerability detection |
| **Documentation** | ~1-2 min | Custom validation, link checking | Docs completeness |
| **Static Analysis** | ~2-4 min | pylint, pydocstyle, radon, xenon | Code complexity |
| **Link Check** | ~1-2 min | markdown-link-check | Link validation |
| **Summary** | ~30 sec | Custom reporting | Status aggregation |

## 🎯 Benefits

- **Quality Assurance**: Catches issues before they reach users
- **Consistency**: Enforces coding standards across all contributors
- **Security**: Prevents vulnerabilities and secret leaks
- **Reliability**: Ensures tools work correctly for all users
- **Documentation**: Maintains comprehensive and accurate guides
- **Maintainability**: Identifies complex code that needs refactoring

## ⚡ Quick Reference

### For Contributors
```bash
# Before pushing, run these checks locally:
black --check --diff .                    # Python formatting
isort --check-only --diff .               # Import sorting
flake8 . --max-line-length=100            # Python linting
find . -name "*.sh" -exec shellcheck {} \; # Shell linting
```

### For Maintainers
- **View CI Status**: Go to Actions tab in GitHub
- **Download Reports**: Check artifacts section for detailed reports
- **Re-run Jobs**: Click "Re-run jobs" button for failed workflows
- **Debug Issues**: Click on individual job names for detailed logs

### Common Commands
```bash
# Fix Python formatting
black .
isort .

# Check security
pip-audit --requirement requirements.txt
safety check --requirement requirements.txt

# Validate shell scripts
shellcheck build_app.sh config_loader.sh

# Check markdown links
markdown-link-check README.md
```

## 🆘 Troubleshooting

### CI Not Running
- Check if workflows are enabled in repository settings
- Verify branch protection rules
- Ensure workflows are in `.github/workflows/` directory

### Workflow Failures
- Check the specific job logs for detailed error messages
- Verify all required files exist
- Ensure proper file permissions
- Check for syntax errors in configuration files

### Performance Issues
- Monitor GitHub Actions usage
- Check for workflow optimization opportunities
- Consider reducing file scope if needed

### Common Error Messages
| Error | Cause | Solution |
|-------|-------|----------|
| `black: code formatting issues` | Python code not formatted | Run `black .` |
| `shellcheck: SC2034` | Unused variable | Remove or use the variable |
| `flake8: E501` | Line too long | Break long lines or ignore |
| `gitleaks: secret detected` | Accidental secret commit | Remove secret and rotate keys |
| `markdown-link-check: 404` | Broken link | Fix or remove the link |

## 📚 Additional Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Python Code Style Guide](https://pep8.org/)
- [Shell Script Best Practices](https://google.github.io/styleguide/shellguide.html)
- [Markdown Style Guide](https://www.markdownguide.org/basic-syntax/)

---

**Need Help?** Check the individual job logs in the GitHub Actions tab for detailed error messages and troubleshooting information.