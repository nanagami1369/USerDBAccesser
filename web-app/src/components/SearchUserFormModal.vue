<template>
  <modal name="search-user-form-modal" id="search-user-form-modal" :height="250">
    <ValidationObserver :immediate="true" v-slot="{ invalid }">
      <form id="search-user-form">
        <h4>アカウントの検索</h4>
        <ValidationProvider :immediate="true" rules="empty">
          <label for="search-user-input-name">名前(部分一致)</label>
          <input
            type="text"
            name="名前"
            id="search-user-input-name"
            v-model="searchUserData.name"
          />
        </ValidationProvider>
        <ValidationProvider :immediate="true" rules="isNone">
          <label>状態</label>
          <div id="avail-radio-button-aria">
            <label bel for="avail-radio-true">
              <input
                type="radio"
                name="avail"
                value="true"
                id="avail-radio-true"
                v-model="searchUserData.avail"
              />有効
            </label>
            <label for="avail-radio-false">
              <input
                type="radio"
                name="avail"
                value="false"
                id="avail-radio-false"
                v-model="searchUserData.avail"
              />無効
            </label>
            <label for="avail-radio-none">
              <input
                type="radio"
                name="avail"
                value="null"
                id="avail-radio-none"
                v-model="searchUserData.avail"
              />状態で検索しない
            </label>
          </div>
        </ValidationProvider>
        <ValidationProvider :immediate="true" rules="isNone">
          <label>権限</label>
          <div id="level-radio-button-aria">
            <label for="level-radio-admin">
              <input
                type="radio"
                name="level"
                value="ADMIN"
                id="level-radio-admin"
                v-model="searchUserData.level"
              />管理者
            </label>
            <label for="level-radio-prem">
              <input
                type="radio"
                name="level"
                value="PREM"
                id="level-radio-prem"
                v-model="searchUserData.level"
              />プレミア
            </label>
            <label for="level-radio-gen">
              <input
                type="radio"
                name="level"
                value="GEN"
                id="level-radio-gen"
                v-model="searchUserData.level"
              />一般
            </label>
            <label for="level-radio-try">
              <input
                type="radio"
                name="level"
                value="TRY"
                id="level-radio-try"
                v-model="searchUserData.level"
              />お試し
            </label>
            <label for="level-radio-none">
              <input
                type="radio"
                name="level"
                value="null"
                id="level-radio-none"
                v-model="searchUserData.level"
              />権限で検索しない
            </label>
          </div>
        </ValidationProvider>
        <button id="send-form-add-user-button" :disabled="!invalid" type="button" @click="submit">
          以下の条件で検索する
        </button>
        <p v-show="!invalid" class="error-message">検索条件が設定されていません</p>
      </form>
    </ValidationObserver>
  </modal>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator'
import { ValidationProvider, ValidationObserver, extend } from 'vee-validate'
import { SearchUserData } from '@/model/SearchUserData'
extend('isNone', value => {
  return value === 'null'
})
extend('empty', value => {
  if (typeof value == undefined) {
    return true
  }
  if (value === null) {
    return true
  }
  if (value === '') {
    return true
  }
  return false
})

@Component({
  components: { ValidationProvider, ValidationObserver }
})
export default class SearchUserFormModal extends Vue {
  public searchUserData: SearchUserData = {
    name: '',
    avail: 'null',
    level: 'null'
  }
  public submit() {
    this.$emit('submit', this.searchUserData)
    this.searchUserData = {
      name: '',
      avail: 'null',
      level: 'null'
    }
    this.$modal.hide('search-user-form-modal')
  }
}
</script>
<style scoped>
#search-user-form {
  display: flex;
  flex-direction: column;
  margin: 10px;
}

.error-message {
  text-decoration: underline;
}
.error-message:not(:empty)::before {
  content: '※ ';
  color: red;
}

#send-form-add-user-button {
  font-size: 1em;
  margin: 0.5rem 5rem;
}

#send-form-add-user-button:disabled {
  background-color: gray;
}
</style>
